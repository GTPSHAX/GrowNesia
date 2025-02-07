#include <Header/Utils/Utils.h>     // Menghubungkan header utils kesini
#include <Header/Player/GrowPlayer.h>
#include <x64-windows/include/enet/enet.h>


std::string Utils::hashWithSHA256(const std::string& data) {
    EVP_MD_CTX* context = EVP_MD_CTX_new(); // Alokasikan konteks hash
    const EVP_MD* md = EVP_sha256();       // Pilih algoritma SHA-256
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLength = 0;

    if (EVP_DigestInit_ex(context, md, nullptr) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to initialize digest context");
    }

    if (EVP_DigestUpdate(context, data.c_str(), data.size()) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to update digest");
    }

    if (EVP_DigestFinal_ex(context, hash, &hashLength) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to finalize digest");
    }

    EVP_MD_CTX_free(context); // Bebaskan memori konteks

    // Ubah hasil hash menjadi string heksadesimal
    std::stringstream ss;
    for (unsigned int i = 0; i < hashLength; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}

void Utils::consoleLog(const OutputType& type, const std::string ctx) {
    switch (type) {
        case ERR: {
            std::cout << "\033[91m[ERROR] " << ctx << "\033[0m" << std::endl; // Bright Red
            break;
        }
        case SUC: {
            std::cout << "\033[92m[SUCCESS] " << ctx << "\033[0m" << std::endl; // Bright Green
            break;
        }
        case INF: {
            std::cout << "\033[94m[INFO] " << ctx << "\033[0m" << std::endl; // Bright Blue
            break;
        }
        case WRN: {
            std::cout << "\033[93m[WARNING] " << ctx << "\033[0m" << std::endl; // Bright Yellow
            break;
        }
        case SEC: {
            std::cout << "\033[95m[SECURITY] " << ctx << "\033[0m" << std::endl; // Bright Magenta
            break;
        }
        case DBG: {
            std::cout << "\033[96m[DEBUG] " << ctx << "\033[0m" << std::endl; // Bright Cyan
            break;
        }
        default: {
            std::cout << "[UNKNOWN] " << ctx << std::endl;
            break;
        }
    }
}
std::string Utils::readFile(const std::string path) {
    // kembalikan error jika path/file ridak ditemukan
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("Path " + path + " not found!");   // Mengembalikan error
    }

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to access " + path + "!");
    }

    return std::string((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>())); // Mengambil secara char dan dibungkus menjadi sebuah string kemudian dikembalikan 
}
void Utils::writeFile(const std::string path, const std::string& ctx) {
    std::ofstream file(path, std::ios::binary); // Inisialisasi file yang akan ditulis

    // Jika gagal diakses akan return error
    if (!file.is_open()) {
        throw std::runtime_error("Failed to access " + path + "!");
    }

    file << ctx;    // Memasukkan kontent kedalam file
    file.close();   // Tutup kembali file
}
void Utils::writeJson(const std::string path, const json& ctx) {
    writeFile(path, ctx.dump(4));
}
int Utils::generateID(GenerateType type) {
    // Kita akan menggunakan switch pada kondisi ini karena lebih cocok daripada menggunakan if else
    // Di switch tidak akan melakukan pengecekan satu persatu seperti if else tetapi langsung di eksekusi jika terdapat key yang cocok

    // Kita muat dulu data terbarunya
    json j = json::parse(readFile(_DATA + "generated-id.json"));    // Kita olah data mentahnya menjadi json

    // kita timpa ulang nilai dari variabel
    GenerateData.account = j.contains("ACCOUNT") ? j["ACCOUNT"].get<int>() : GenerateData.account;
    GenerateData.world = j.contains("WORLD") ? j["WORLD"].get<int>() : GenerateData.world;
    GenerateData.guild = j.contains("GUILD") ? j["GUILD"].get<int>() : GenerateData.world;

    int returned = 0;
    switch (type)
    {
        case ACCOUNT: {
            GenerateData.account++;
            j["ACCOUNT"] = GenerateData.account;
            returned = GenerateData.account;
            break;
        }
        case WORLD: {
            GenerateData.world++;
            j["WORLD"] = GenerateData.world;
            returned = GenerateData.world;
            break;
        }
        case GUILD: {
            GenerateData.guild++;
            j["GUILD"] = GenerateData.guild;
            returned = GenerateData.guild;
            break;
        }
        default: {
            throw std::runtime_error("Invalid GenerateType");   // Melempar/mengembalikan error dengan pesan "Invalid GenerateType"
            break;
        }
    }

    writeJson(_DATA + "generated-id.json", j);
    return returned;
}
std::string Utils::parseMoney(int value) {
    std::stringstream ss;
    ss.imbue(std::locale("en_US.UTF-8")); // Set locale untuk format angka
    ss << std::fixed << value; // Masukkan angka ke stream

    return ss.str(); // Mengembalikan string yang diformat
}
std::vector<std::string> Utils::explode(const std::string& delimiter, const std::string& str) {
    // Variabel untuk menyimpan hasil potongan string
    std::vector<std::string> result;

    // Posisi awal pencarian dalam string
    size_t start = 0;

    // Temukan posisi delimiter pertama dalam string
    size_t end = str.find(delimiter);

    // Loop selama delimiter masih ditemukan dalam string
    while (end != std::string::npos) {
        // Ambil potongan string dari posisi start hingga sebelum delimiter
        std::string potongan = str.substr(start, end - start);

        // Masukkan potongan string ke dalam hasil
        result.push_back(potongan);

        // Geser posisi start ke setelah delimiter yang ditemukan
        start = end + delimiter.length();

        // Cari delimiter berikutnya setelah posisi start yang baru
        end = str.find(delimiter, start);
    }

    // Tambahkan sisa string setelah delimiter terakhir
    std::string sisa = str.substr(start);
    result.push_back(sisa);

    // Kembalikan hasil potongan string
    return result;
}
json Utils::readJson(const std::string path) {
    return json::parse(readFile(path));
}