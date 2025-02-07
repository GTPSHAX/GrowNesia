
const notif = document.querySelector(".notif");
const params = new URLSearchParams(window.location.search);

const growid = params.get('growid') ? decodeURIComponent(params.get('growid')) : '';
const password = params.get('password') ? decodeURIComponent(params.get('password')) : '';
const success = params.get('status');

// Mengisi nilai input jika parameter tersedia
if (growid) document.getElementById('login-name').value = growid;
if (password) document.getElementById('password').value = password;

// Menampilkan notifikasi jika status=false
if (success == 'false') {
notif.innerHTML = `<div class="animate-bounce flex gap-1 items-center p-4 mb-4 text-sm text-red-800 rounded-lg bg-red-50 dark:bg-gray-800 dark:text-red-400" role="alert">
<svg class="flex-shrink-0 inline w-4 h-4 me-3" aria-hidden="true" xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 20 20">
<path d="M10 .5a9.5 9.5 0 1 0 9.5 9.5A9.51 9.51 0 0 0 10 .5ZM9.5 4a1.5 1.5 0 1 1 0 3 1.5 1.5 0 0 1 0-3ZM12 15H8a1 1 0 0 1 0-2h1v-3H8a1 1 0 0 1 0-2h2a1 1 0 0 1 1 1v4h1a1 1 0 0 1 0 2Z"/>
</svg>
<span class="font-medium">Failed:</span> Miss match GrowID or Password
</div>`;
}

document.querySelector('form').addEventListener("submit", async (e) => {
    e.preventDefault(); // Mencegah form submit bawaan
    
    notif.innerHTML = ""; // Reset notifikasi
    
    const growid = document.getElementById('login-name').value.trim();
    const pass = document.getElementById('password').value.trim();

    // Validasi sederhana
    if (growid.length < 3 || (pass.length < 4 && pass.length)) {
        if (growid.length < 3) notif.innerHTML = `
            <div class="animate-bounce flex items-center p-4 mb-4 text-sm text-red-800 rounded-lg bg-red-50 dark:bg-gray-800 dark:text-red-400" role="alert">
                <svg class="flex-shrink-0 inline w-4 h-4 me-3" aria-hidden="true" xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 20 20">
                    <path d="M10 .5a9.5 9.5 0 1 0 9.5 9.5A9.51 9.51 0 0 0 10 .5ZM9.5 4a1.5 1.5 0 1 1 0 3 1.5 1.5 0 0 1 0-3ZM12 15H8a1 1 0 0 1 0-2h1v-3H8a1 1 0 0 1 0-2h2a1 1 0 0 1 1 1v4h1a1 1 0 0 1 0 2Z"/>
                </svg>
                <span class="font-medium">Error:</span> GrowID must be at least 3 characters long.
            </div>`;
        else notif.innerHTML = `
            <div class="animate-bounce flex items-center p-4 mb-4 text-sm text-red-800 rounded-lg bg-red-50 dark:bg-gray-800 dark:text-red-400" role="alert">
                <svg class="flex-shrink-0 inline w-4 h-4 me-3" aria-hidden="true" xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 20 20">
                    <path d="M10 .5a9.5 9.5 0 1 0 9.5 9.5A9.51 9.51 0 0 0 10 .5ZM9.5 4a1.5 1.5 0 1 1 0 3 1.5 1.5 0 0 1 0-3ZM12 15H8a1 1 0 0 1 0-2h1v-3H8a1 1 0 0 1 0-2h2a1 1 0 0 1 1 1v4h1a1 1 0 0 1 0 2Z"/>
                </svg>
                <span class="font-medium">Error:</span> Password must be at least 8 characters long.
            </div>`;
        return;
    }
    if (!isAlphaNumericOnly(growid) || (!isAlphaNumericOnly(pass, true) && pass.length)) {
        notif.innerHTML = `
            <div class="animate-bounce flex items-center p-4 mb-4 text-sm text-red-800 rounded-lg bg-red-50 dark:bg-gray-800 dark:text-red-400" role="alert">
                <svg class="flex-shrink-0 inline w-4 h-4 me-3" aria-hidden="true" xmlns="http://www.w3.org/2000/svg" fill="currentColor" viewBox="0 0 20 20">
                    <path d="M10 .5a9.5 9.5 0 1 0 9.5 9.5A9.51 9.51 0 0 0 10 .5ZM9.5 4a1.5 1.5 0 1 1 0 3 1.5 1.5 0 0 1 0-3ZM12 15H8a1 1 0 0 1 0-2h1v-3H8a1 1 0 0 1 0-2h2a1 1 0 0 1 1 1v4h1a1 1 0 0 1 0 2Z"/>
                </svg>
                <span class="font-medium">Error:</span> Invalid GrowID or Password. Only alphanumeric characters are accepted.
            </div>`;
        return;
    }
    document.querySelector('form').submit();
});

function isAlphaNumericOnly(input, isPass = false) {
    input = input.replace(/\s+/g, '');

    // Regular expressions for validation
    const alphaNumericPattern = /^[A-Za-z0-9]+$/; // Alphanumeric only
    const passwordPattern = /^[A-Za-z0-9!@#$%^&*()_+\-=\[\]{};':"\\|,.<>\/?`~]+$/; // Alphanumeric + symbols

    // Use appropriate regex based on isPass flag
    const pattern = isPass ? passwordPattern : alphaNumericPattern;

    // Test the input against the chosen regex
    return pattern.test(input);
}