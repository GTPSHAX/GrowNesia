module.exports = {
    host: "auto", // Web server ip listener (write "auto" will automatically filled with your current ipv4 address)
    port: [[443, "HTTPS"]], // Web server port listener (support multi port)
    server_data: {
        loginurl: "gp.gtps.pw", // Domain for login url, DM telegram @OxygenBro for free configured domain loginurl 
        ip: "auto", // ENetServer IP (write "auto" will automatically filled with your current ipv4 address)
        master_port: 17000, // Master ENetServer port
        type: 1, // Status server 1 (public) 0 (private)
        message: "`5The servers is currently undergoing maintenance. We will be back soon.",
        meta: "GP",

        // Beta server
        beta_ip: "983.892.93.2", // ENetServer IP (write "auto" will automatically filled with your current ipv4 address)
        beta_master_port: 17092, // Master ENetServer port
        beta_type: 1, // Status server 1 (public) 0 (private)
        beta_loginurl: "gp.gtps.pw", // Domain for login url, DM telegram @OxygenBro for free configured domain loginurl
    },
    security: {
        limiter: {
            reset: 10000, // Reset data in MS
            limit: 200, // Limit request
        }
    },
    cdn: "https://ubistatic-a.akamaihd.net/0098/031012202407", // Cdn pengganti jika ada missing cache
};