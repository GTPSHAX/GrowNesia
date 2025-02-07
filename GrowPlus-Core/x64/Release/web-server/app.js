const express = require("express");
const bodyParser = require('body-parser');
const path = require("path");
const {rateLimit } = require("express-rate-limit");
const { exec } = require('child_process');
const fs = require("fs");
const https = require("https");

const cnf = require("./config");
const { runInNewContext } = require("vm");
const log = console.log;
const app = express();

// close port
let closedPort = [];

// SSL https gt
const crt = {
    "cert": fs.readFileSync("./cert/gt.crt", "utf-8"),
    "key": fs.readFileSync("./cert/gt.key", "utf-8")
}

// Setting rateLimit
app.set('trust proxy', 1);
const limiter = rateLimit({
    windowMs: cnf.security.limiter.reset,
    limit: cnf.security.limiter.limit,
    statusCode: 404,
    handler: () => {
        let server = app.get("server");
        server = server.address();
        console.log(`[ALERT] Detected suspicious request in host ${server.address} in port ${server.port}. Closing the port...`);
        closePortTemporarily(server.port, cnf.security.limiter.reset); // Tutup port selama 10 detik
    }
});
app.use(limiter);

app.use(async function(req, res, next) {
    log(`[${req.ip}] Request to: ${req.url} [${req.method}]`);

    res.header('Access-Control-Allow-Origin', '*');
    res.header(
        'Access-Control-Allow-Headers',
        'Origin, X-Requested-With, Content-Type, Accept',
    );
    next();
});
app.use(bodyParser.urlencoded({ extended: true }));

// Ambil data server_data
app.post('/growtopia/server_data.php', async (req, res) => {
    // Cek apakah host yang mengakses sesuai dengan daftar izin
    if (!["www.growtopia1.com", "www.growtopia2.com", "growtopia1.com", "growtopia2.com", cnf.server_data.loginurl].includes(req.hostname)) {
        res.status(403).send("");
    } else {
        // Data server yang akan dikirimkan
        const content = `server|${cnf.server_data.ip}
port|${cnf.server_data.master_port}
type|${cnf.server_data.type}
beta_server|${cnf.server_data.beta_ip}
beta_port|${cnf.server_data.beta_master_port}
beta_loginurl|${cnf.server_data.beta_loginurl}
beta_type2|${cnf.server_data.beta_type}
#maint|${cnf.server_data.message}
loginurl|${cnf.server_data.loginurl}
meta|${cnf.server_data.meta}
RTENDMARKERBS1001`;

        // Kirimkan data dengan Content-Type: text/plain
        log(content);
        res
            .header("Content-Type", "text/plain")
            .send(content);
    }
});
// server host
app.all('/host', async (req, res) => {
    const content = `${cnf.server_data.ip} growtopia1.com
${cnf.server_data.ip} growtopia2.com
${cnf.server_data.ip} www.growtopia1.com
${cnf.server_data.ip} www.growtopia2.com
${cnf.server_data.beta_ip} beta.growtopiagame.com`;
    res.header("Content-Type", "text/plain").send(content);
});

// Login handler
app.post('/player/login/dashboard', (req, res) => {
    try {
        res.status(200).redirect("/login/");
    } catch (error) {
        console.error(error);
        res.sendStatus(404);
    }
});
app.post('/player/growid/checktoken', (req,res)=>{
    res.send(
        `{"status":"success","message":"Account Validated.","token":"${req.body.refreshToken}","url":"","accountType":"growtopia"}`,
    );
})
app.all('/player/growid/login/validate', (req, res) => {
    try {
        let success = false;
        req.body.growId = req.body.growId.replace(/\s+/g, '');
        req.body.password = req.body.password.replace(/\s+/g, '');
        if (req.body.growId && req.body.password) {
            if (fs.existsSync(path.join(__dirname, `../database/player/${req.body.growId.toUpperCase()}.json`))) {
                const player = JSON.parse(fs.readFileSync(path.join(__dirname, `../database/player/${req.body.growId.toUpperCase()}.json`)));
                if (player["data"]["tankIDPass"] == req.body.password) success = true;
            }
        }
        else if (req.body.growId && !req.body.password) {
            if (!fs.existsSync(path.join(__dirname, `../database/player/${req.body.growId.toUpperCase()}.json`))) {
                success = true;
            }
        }

        if (success) {
            // Extracting data from the request body
            const _token = req.body._token;
            const growId = req.body.growId;
            const password = req.body.password;

            let token = Buffer.from(
                `_token=${_token}&growId=${growId}&password=${password}`,
            ).toString('base64');

            res.send(
                `{"status":"success","message":"Account Validated.","token":"${token}","url":"","accountType":"growtopia"}`,
            );
            return;
        }
        res.redirect(`/login/index.html?growid=${req.body.growId}&password=${req.body.password}&status=false`);
    } catch (error) {
        console.error(error);
        res.sendStatus(404);
    }
});
app.post('/player/validate/close', function (req, res) {
    res.send('<script>window.close();</script>');
});

// Web root
app.use(express.static(path.resolve (__dirname, "public")));
app.get("/cache/*", (req, res) => {
    const targetUrl = cnf.cdn + req.url;
    log(`Missing cache detected, redirecting to ${targetUrl}`);
    
    https.get(targetUrl, (response) => {
        if (response.statusCode === 200) {
            res.redirect(targetUrl);
        } else {
            log(`Failed to fetch ${targetUrl}`);
            res.status(200).send('');
        }
    }).on('error', (e) => {
        log(`Failed to fetch ${targetUrl}: ${e.message}`);
        res.status(200).send('');
    });
});

// Start server
(async()=>{
    if (cnf.host.toUpperCase() == "AUTO") cnf.host = (await getPublicIPv4());
    if (cnf.server_data.ip.toUpperCase() == "AUTO") cnf.server_data.ip = (await getPublicIPv4());
    if (cnf.server_data.beta_ip.toUpperCase() == "AUTO") cnf.server_data.beta_ip = (await getPublicIPv4()); 
    cnf.port.forEach(async port => {
        log("Starting "+port[1]+" server with port: " + port[0])
        if (port[1].toUpperCase() == "HTTPS") {
            const server = https.createServer(crt, app).listen(port[0], cnf.host, () => {
                log('Secure server running at https://localhost:'+port[0]+'/');
                app.set('server', server); // Menyimpan server instance di app
            });
        }
        else {
            const server = app.listen(port[0], cnf.host, () => {
                log('Server running at http://localhost:'+port[0]+'/');
                app.set('server', server); // Menyimpan server instance di app
            })
        }
    });
})();


// Fungsi untuk mendapatkan IP publik
async function getPublicIPv4() {
    try {
        const response = await fetch('https://api.ipify.org?format=json');
        const data = await response.json();
        return data.ip; // Public IPv4 address
    } catch (error) {
        console.error('Error fetching public IPv4:', error);
    }
}

// Fungsi untuk menutup port menggunakan netsh di Windows
async function closePort(port) {
    const command = `netsh advfirewall firewall delete rule name="${port}"`;
    
    exec(command, (error, stdout, stderr) => {
        if (error) {
            // Jika terjadi kesalahan, tampilkan pesan kesalahan
            console.error(`Error while deleting inbound rule for port ${port}:`, stderr || error.message);
        } else {
            // Jika berhasil, tampilkan pesan konfirmasi
            console.log(`Inbound rule for port ${port} has been removed.`);
        }
    });

}

// Fungsi untuk membuka port kembali menggunakan netsh di Windows
async function openPort(port) {
    console.log(`Opening port ${port}...`);
    const command = `netsh advfirewall firewall add rule name="${port}" dir=in action=allow enable=yes protocol=tcp profile=private,domain,public localport=${port}`;

    exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error while adding inbound rule for port ${port}:`, error);
        } else {
            console.log(`Port ${port} is now allowed for inbound connections.`);
        }
    });
}

// Penutupan port sementara
async function closePortTemporarily(port, duration) {
    if (closedPort.includes(port)) return;
    closedPort.push(port);
    await closePort(port);
    setTimeout(async () => {
        await openPort(port);
        closedPort = closedPort.filter(i => i != port);
    }, duration); // Buka port kembali setelah waktu tertentu
};