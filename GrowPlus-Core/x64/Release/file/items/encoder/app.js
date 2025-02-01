// KODE INI TIDAK UNTUK DIPERJUAL BELIKAN, DI CIPTAKAN OLEH GuckTubeYT DAN DI UBAH KE NODEJS OLEH GROWPLUS DEVELOPER TEAM
const fs = require("node:fs");
const path = require("node:path");

var items_secret_key = "PBG892FXX982ABC*"
var data_json = {}
var encoded_buffer_file = [];
let msg = "";

const byteToHex = [];

for (let n = 0; n <= 0xff; ++n) {
    const hexOctet = n.toString(16).padStart(2, "0");
    byteToHex.push(hexOctet);
}

/**
 * @param {ArrayBuffer} arrayBuffer
 * @return {string}
 */
function hex(arrayBuffer, is_without_space) {
    const buff = new Uint8Array(arrayBuffer);
    const hexOctets = [];
    for (let i = 0; i < buff.length; ++i) hexOctets.push(byteToHex[buff[i]]);

    return hexOctets.join(is_without_space ? "" : " ");
}

/**
 * @param {ArrayBuffer} buffer
 * @param {number} pos
 * @param {number} len
 * @return {string}
 */
function read_buffer_number(buffer, pos, len) {
    let value = 0;
    for (let a = 0; a < len; a++) value += buffer[pos + a] << (a * 8)

    return value;
}

/**
 * @param {number} pos
 * @param {number} len
 * @param {number} value
 */
function write_buffer_number(pos, len, value) {
    for (let a = 0; a < len; a++) {
        encoded_buffer_file[pos + a] = (value >> (a * 8)) & 255;
    }
}

function write_buffer_string(pos, len, value, using_key, item_id) {
    for (let a = 0; a < len; a++) {
        if (using_key) encoded_buffer_file[pos + a] = value.charCodeAt(a) ^ (items_secret_key.charCodeAt((a + item_id) % items_secret_key.length))
        else encoded_buffer_file[pos + a] = value.charCodeAt(a)
    }
}

function hash_buffer(buffer, element, text) {
    var hash = 0x55555555;
    var toBuffer = new Uint8Array(buffer);
    for (let a = 0; a < toBuffer.length; a++) hash = (hash >>> 27) + (hash << 5) + toBuffer[a]
    msg = text + hash
}

/**
 * Convert a hex string to an ArrayBuffer.
 * 
 * @param {string} hexString - hex representation of bytes
 * @return {ArrayBuffer} - The bytes in an ArrayBuffer.
 */
function hexStringToArrayBuffer(pos, hexString) { //https://gist.github.com/don/871170d88cf6b9007f7663fdbc23fe09
    // remove the space
    hexString = hexString.replace(/ /g, '');
    if (hexString.length % 2 != 0) console.log('WARNING: expecting an even number of characters in the hexString');
    
    // check for some non-hex characters
    var bad = hexString.match(/[G-Z\s]/i);
    if (bad) console.log('WARNING: found non-hex characters', bad);    

    // convert the octets to integers
    var integers = hexString.match(/[\dA-F]{2}/gi).map(function(s) {
        encoded_buffer_file[pos++] = parseInt(s, 16)
    });

    return integers
}

/**
 * @param {ArrayBuffer} buffer
 * @param {number} pos
 * @param {number} len
 * @param {boolean} using_key
 * @param {number} item_id
 */
function read_buffer_string(buffer, pos, len, using_key, item_id) {
    var result = "";
    if (using_key) for (let a = 0; a < len; a++) result += String.fromCharCode(buffer[a + pos] ^ items_secret_key.charCodeAt((item_id + a) % items_secret_key.length))
    else for (let a = 0; a < len; a++) result += String.fromCharCode(buffer[a + pos])
    
    return result;
}

function check_last_char(dest, src) {
    return dest[dest.length - 1] == src
}

function process_item_encoder(result) {
    var mem_pos = 6;

    write_buffer_number(0, 2, result.version)
    write_buffer_number(2, 4, result.item_count)
    for (let a = 0; a < result.item_count; a++) {
        write_buffer_number(mem_pos, 4, result.items[a].item_id);
        mem_pos += 4;
        encoded_buffer_file[mem_pos++] = result.items[a].editable_type
        encoded_buffer_file[mem_pos++] = result.items[a].item_category
        encoded_buffer_file[mem_pos++] = result.items[a].action_type
        encoded_buffer_file[mem_pos++] = result.items[a].hit_sound_type
        write_buffer_number(mem_pos, 2, result.items[a].name.length);
        mem_pos += 2;
        write_buffer_string(mem_pos, result.items[a].name.length, result.items[a].name, 1, result.items[a].item_id)
        mem_pos += result.items[a].name.length
        write_buffer_number(mem_pos, 2, result.items[a].texture.length);
        mem_pos += 2;
        write_buffer_string(mem_pos, result.items[a].texture.length, result.items[a].texture)
        mem_pos += result.items[a].texture.length
        write_buffer_number(mem_pos, 4, result.items[a].texture_hash)
        mem_pos += 4;
        encoded_buffer_file[mem_pos++] = result.items[a].item_kind
        write_buffer_number(mem_pos, 4, result.items[a].val1)
        mem_pos += 4;
        encoded_buffer_file[mem_pos++] = result.items[a].texture_x
        encoded_buffer_file[mem_pos++] = result.items[a].texture_y
        encoded_buffer_file[mem_pos++] = result.items[a].spread_type
        encoded_buffer_file[mem_pos++] = result.items[a].is_stripey_wallpaper
        encoded_buffer_file[mem_pos++] = result.items[a].collision_type

        if (check_last_char(result.items[a].break_hits.toString(), "r")) encoded_buffer_file[mem_pos++] = Number(result.items[a].break_hits.toString().slice(0, -1))
        else encoded_buffer_file[mem_pos++] = Number(result.items[a].break_hits) * 6

        write_buffer_number(mem_pos, 4, result.items[a].drop_chance)
        mem_pos += 4;
        encoded_buffer_file[mem_pos++] = result.items[a].clothing_type
        write_buffer_number(mem_pos, 2, result.items[a].rarity)
        mem_pos += 2;
        
        encoded_buffer_file[mem_pos++] = result.items[a].max_amount
        write_buffer_number(mem_pos, 2, result.items[a].extra_file.length);
        mem_pos += 2;
        write_buffer_string(mem_pos, result.items[a].extra_file.length, result.items[a].extra_file)
        mem_pos += result.items[a].extra_file.length
        write_buffer_number(mem_pos, 4, result.items[a].extra_file_hash)
        mem_pos += 4;
        write_buffer_number(mem_pos, 4, result.items[a].audio_volume)
        mem_pos += 4;
        write_buffer_number(mem_pos, 2, result.items[a].pet_name.length);
        mem_pos += 2;
        write_buffer_string(mem_pos, result.items[a].pet_name.length, result.items[a].pet_name)
        mem_pos += result.items[a].pet_name.length
        write_buffer_number(mem_pos, 2, result.items[a].pet_prefix.length);
        mem_pos += 2;
        write_buffer_string(mem_pos, result.items[a].pet_prefix.length, result.items[a].pet_prefix)
        mem_pos += result.items[a].pet_prefix.length
        write_buffer_number(mem_pos, 2, result.items[a].pet_suffix.length);
        mem_pos += 2;
        write_buffer_string(mem_pos, result.items[a].pet_suffix.length, result.items[a].pet_suffix)
        mem_pos += result.items[a].pet_suffix.length
        write_buffer_number(mem_pos, 2, result.items[a].pet_ability.length);
        mem_pos += 2;
        write_buffer_string(mem_pos, result.items[a].pet_ability.length, result.items[a].pet_ability)
        mem_pos += result.items[a].pet_ability.length
        encoded_buffer_file[mem_pos++] = result.items[a].seed_base
        encoded_buffer_file[mem_pos++] = result.items[a].seed_overlay
        encoded_buffer_file[mem_pos++] = result.items[a].tree_base
        encoded_buffer_file[mem_pos++] = result.items[a].tree_leaves
        encoded_buffer_file[mem_pos++] = result.items[a].seed_color.a
        encoded_buffer_file[mem_pos++] = result.items[a].seed_color.r
        encoded_buffer_file[mem_pos++] = result.items[a].seed_color.g
        encoded_buffer_file[mem_pos++] = result.items[a].seed_color.b
        encoded_buffer_file[mem_pos++] = result.items[a].seed_overlay_color.a
        encoded_buffer_file[mem_pos++] = result.items[a].seed_overlay_color.r
        encoded_buffer_file[mem_pos++] = result.items[a].seed_overlay_color.g
        encoded_buffer_file[mem_pos++] = result.items[a].seed_overlay_color.b
        write_buffer_number(mem_pos, 4, 0); // skipping ingredients
        mem_pos += 4;
        write_buffer_number(mem_pos, 4, result.items[a].grow_time);
        mem_pos += 4;
        write_buffer_number(mem_pos, 2, result.items[a].val2);
        mem_pos += 2;
        write_buffer_number(mem_pos, 2, result.items[a].is_rayman);
        mem_pos += 2;
        write_buffer_number(mem_pos, 2, result.items[a].extra_options.length);
        mem_pos += 2;
        write_buffer_string(mem_pos, result.items[a].extra_options.length, result.items[a].extra_options)
        mem_pos += result.items[a].extra_options.length
        write_buffer_number(mem_pos, 2, result.items[a].texture2.length);
        mem_pos += 2;
        write_buffer_string(mem_pos, result.items[a].texture2.length, result.items[a].texture2)
        mem_pos += result.items[a].texture2.length
        write_buffer_number(mem_pos, 2, result.items[a].extra_options2.length);
        mem_pos += 2;
        write_buffer_string(mem_pos, result.items[a].extra_options2.length, result.items[a].extra_options2)
        mem_pos += result.items[a].extra_options2.length
        hexStringToArrayBuffer(mem_pos, result.items[a].data_position_80)
        mem_pos += 80;
        if (result.version >= 11) {
            write_buffer_number(mem_pos, 2, result.items[a].punch_options.length);
            mem_pos += 2;
            write_buffer_string(mem_pos, result.items[a].punch_options.length, result.items[a].punch_options)
            mem_pos += result.items[a].punch_options.length
        }
        if (result.version >= 12) {
            hexStringToArrayBuffer(mem_pos, result.items[a].data_version_12)
            mem_pos += 13;
        }
        if (result.version >= 13) {
            write_buffer_number(mem_pos, 4, result.items[a].int_version_13)
            mem_pos += 4;
        }
        if (result.version >= 14) {
            write_buffer_number(mem_pos, 4, result.items[a].int_version_14)
            mem_pos += 4;
        }
        if (result.version >= 15) {
            hexStringToArrayBuffer(mem_pos, result.items[a].data_version_15)
            mem_pos += 25;
            write_buffer_number(mem_pos, 2, result.items[a].str_version_15.length);
            mem_pos += 2;
            write_buffer_string(mem_pos, result.items[a].str_version_15.length, result.items[a].str_version_15)
            mem_pos += result.items[a].str_version_15.length
        }
        if (result.version >= 16) {
            write_buffer_number(mem_pos, 2, result.items[a].str_version_16.length);
            mem_pos += 2;
            write_buffer_string(mem_pos, result.items[a].str_version_16.length, result.items[a].str_version_16)
            mem_pos += result.items[a].str_version_16.length
        }
        if (result.version >= 17) {
            write_buffer_number(mem_pos, 4, result.items[a].int_version_17)
            mem_pos += 4;
        }
        if (result.version >= 18) {
            write_buffer_number(mem_pos, 4, result.items[a].int_version_18)
            mem_pos += 4;
        }
        if (result.version >= 19) {
            write_buffer_number(mem_pos, 4, result.items[a].int_version_19)
            mem_pos += 9;
        }
    }
}

/**
 * @param {Blob} file
 * @param {boolean} using_editor
 */

function item_encoder(file) {
    encoded_buffer_file = [];

    process_item_encoder(JSON.parse(file));

    hash_buffer(encoded_buffer_file, "items_dat_hash_1", "Encoded Items dat Hash: ")

    return encoded_buffer_file;
}

// addCommand("jsonDat", async (data) => {
//     try {
//         data = data[0];
//         if (!data.buffer) {
//             throw new Error("No buffer provided.");
//         }
        
        
//         const jsonData = item_encoder(data.buffer.toString('utf-8'), false);

//         return jsonData;
//     } catch (error) {
//         return { "System": `Error processing JSON: ${error.message}` };
//     }
// });

(async()=>{
    const data = fs.readFileSync(path.join(process.argv[2]));
    const jsonData = item_encoder(data);
    fs.writeFileSync(path.join(__dirname, "..", ".temp", "items.dat"), Buffer.from(jsonData));
})();