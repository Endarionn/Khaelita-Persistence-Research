# 🧬 Khaelita Persistence Mechanisms Research

A step-by-step technical analysis of **Persistence** mechanisms and their implementation within the Windows environment. This project documents various methods to maintain a foothold on a system, ranging from simple directory-based execution to advanced WMI event subscriptions.

---

## ⚙️ Overview
**Khaelita Persistence Research** is a security project designed to explore how applications can achieve automatic execution across user sessions. It covers standard user-level persistence, registry-based triggers, and "fileless" WMI techniques, providing both offensive proofs-of-concept and defensive mitigation strategies.

---

## 🛠️ Step-by-Step Research Process

### 📂 Step 1: Startup Folder Persistence
**Hypothesis:** If an application copies itself into the Windows Startup folder, it will execute automatically upon user logon without requiring special permissions.

**Action:** Developed a Python-based script that replicates itself into `%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup`.

**Result:** Successful ✅

**Technical Finding:** Windows automatically executes all entries within this directory when `explorer.exe` initializes. This provides a low-privilege persistence path.

**Limitations:**
* Easily visible via Task Manager (Startup tab).
* Leaves a clear file artifact on the disk.
* Limited to the current user context.

### 🔑 Step 2: Registry Run Key Persistence
**Hypothesis:** Adding an executable path to Windows Registry Run keys will trigger execution at user logon, making it slightly more "hidden" than a folder shortcut.

**Action:** Inserted the payload path into `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`.

**Result:** Successful ✅

**Technical Finding:** The Windows logon process evaluates Run keys and executes listed binaries. With administrative privileges, this can be elevated to system-wide using `HKEY_LOCAL_MACHINE`.

**Limitations:**
* Highly monitored by modern EDR/Antivirus solutions.
* Leaves explicit forensic evidence in the registry.

### 📡 Step 3: WMI Event Subscription Persistence
**Hypothesis:** WMI event subscriptions can trigger payload execution based on system events (like idle time or system startup) without relying on traditional "Run" keys.

**Action:** Implemented a persistence chain using an **Event Filter**, an **Event Consumer**, and a **Filter-to-Consumer Binding**.

**Result:** Successful ✅

**Technical Finding:** This method enables "fileless" execution by embedding logic within the WMI repository. Payloads can be triggered by custom system events, making it much harder to detect via standard tools.

**Limitations:**
* Typically requires administrative privileges.
* Detectable through advanced forensic analysis of WMI repositories (`Objects.data`).

### 🎭 Step 4: DLL Hijacking (Spotify Case Study)
**Hypothesis:** Placing a crafted DLL in an application's directory can override the system DLL loading order and enable stealthy code execution.

**Action:** Identified that `Spotify.exe` loads `version.dll` via a relative path. Created a proxy DLL using C++ (function forwarding) and placed it alongside the binary.

**Result:** Successful ✅

**Technical Finding:** Applications that do not enforce absolute paths for DLL loading are vulnerable. This method allows the payload to run inside a "trusted" process, bypassing simple process-name-based detection.

**Key Insight:** Enables stealthy execution and can be extended to fileless delivery using shellcode loaders.

---

## 🛠️ Tools & Techniques

| Category | Tools / Languages |
| :--- | :--- |
| **Compiler** | MSVC (cl.exe), Visual Studio |
| **Dynamic Analysis** | Sysinternals Process Monitor (ProcMon) |
| **Registry Analysis** | Regedit |
| **Languages** | C, C++, C#, Python |

---

## 🔡 How to Defend Against These Techniques

* **Use Absolute Paths:** Always resolve system DLLs using `GetSystemDirectory` to prevent hijacking.
* **Monitor Startup Entries:** Regularly inspect Startup folders and Registry Run keys for unauthorized paths.
* **WMI Auditing:** Query WMI subscriptions via PowerShell (`Get-WmiObject` / `Get-CimInstance`) to look for suspicious consumers.
* **Endpoint Protection:** Utilize modern EDR solutions that monitor for behavioral anomalies like "unusual process startup."

---

## ⚠️ Legal Notice
This software is provided for **educational, ethical research, and awareness purposes only**. Understanding how Windows security architecture works is vital for developing more resilient software. The author assumes no responsibility for misuse.

## 📄 License
MIT License — Research, learn, and secure.

## 🧠 About
Created by **Yağız Atalay** under the **Khaelita** security research label. A study on Windows Internals, persistence techniques, and real-world detection surfaces.
