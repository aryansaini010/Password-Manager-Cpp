# Password Manager Interview Prep Guide

## 1. Project Overview
This is a secure, offline password manager built from scratch using **C++** and the **Qt Framework**. It encrypts your sensitive data on your device, guarding it behind a single master password.

### Core Technologies
- **C++17**: Programming Language.
- **Qt 5 / Qt 6**: For GUI (Widgets) and cryptographic features (`QMessageAuthenticationCode`, `QCryptographicHash`, `QRandomGenerator`).
- **CMake**: Build system used to configure and compile the project.

---

## 2. Architecture & Components

The codebase is split into specific responsibilities to follow good Software Engineering practices (like Separation of Concerns).

### A. Data Layer (`Entry.h`)
A simple structure to hold password records.
- Converts records to and from JSON using Qt's `QJsonObject`.
- Auto-generates unique IDs using `QUuid`.

### B. Security Layer (`CryptoUtils.h / .cpp`)
Provides all cryptographic functions. This is a critical part to highlight in your interview.
- **Key Derivation (PBKDF2-HMAC-SHA256)**: We don't store the master password. We take the master password and a generated salt, and pass it through a hashing function 10,000 times to generate a secure 32-byte (256-bit) encryption key.
- **Encryption Algorithm**: Implements a Stream Cipher algorithm in Counter (CTR) mode using SHA-256.
- **Authentication (Encrypt-then-MAC)**: Appends a 32-byte HMAC (Hash-Based Message Authentication Code) to the encrypted data. Before decrypting, the system verifies the MAC. This prevents attackers from tampering with the vault file.

### C. Storage Layer (`StorageManager.h / .cpp`)
Manages the encrypted vault file (stored in the system's AppData directory).
- Handles reading the vault file.
- Unlocks the vault by reading the unencrypted salt, deriving the key, and verifying/decrypting the payload.
- Provides operations like `addEntry`, `updateEntry`, `deleteEntry`, and automatically re-encrypts and saves the file when data changes.

### D. GUI Layer (View & Controllers)
- **`LoginDialog`**: The "Wall". Checks if the vault exists. If not, it asks you to set a Master Password. If it does exist, it asks you to unlock it.
- **`MainWindow`**: A table view (`QTableWidget`) showing your websites and usernames. Passwords are hidden by default, but you can toggle visibility or copy them to the clipboard.
- **`AddEditDialog`**: A form to add or edit an entry. It includes a password generator that uses `QRandomGenerator` to create strong, secure passwords.

---

## 3. Interview Talking Points
If an interviewer asks you about this project, mention these key features:

* **"I used Encrypt-then-MAC for authentication."** Explain that you didn't just encrypt the file, you also added a cryptographic signature (HMAC). This ensures that if the file gets corrupted or modified by malware, the app will reject it instead of crashing.
* **"I didn't invent my own crypto, but I built the modes myself."** Mention that you used Qt's proven primitives (SHA-256) but assembled the PBKDF2 key derivation and CTR-mode stream cipher manually to avoid bulky external dependencies like OpenSSL.
* **"I used JSON for the underlying data structure."** Emphasize that the unencrypted data is JSON, making it easily extensible, but it is never written to disk in plain text.
* **"I designed the UI purely in C++."** Instead of using Qt Designer (`.ui` files), you coded the layouts (`QVBoxLayout`, `QHBoxLayout`) manually. This shows a deep understanding of layout management.

---

## 4. How to Build & Run
To run this on your Windows machine, you will need **Qt** and **CMake** installed.

1. **Install Qt Open Source**: Download the Qt Online Installer from the official website. When installing, select **Qt 6.x** for Desktop MSVC or MinGW.
2. **Install CMake**: Download and install CMake.
3. **Open the Project**:
   - Open **Qt Creator** (installed with Qt).
   - Go to `File` -> `Open File or Project...`
   - Select the `CMakeLists.txt` inside the `Password Manager` folder.
   - Qt Creator will prompt you to configure the project. Select the Kit you installed (e.g., Desktop Qt 6 MinGW).
   - Click the green **Run (Play)** button in the bottom left corner to build and start the app.
