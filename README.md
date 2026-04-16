# 🛠️ Step-by-Step Research Process

---

## ## Step 1: Startup Folder Persistence

**Hypothesis:** If an application copies itself into the Windows Startup folder, it will execute automatically upon user logon.

**Action:** Developed a Python-based script that replicates itself into:  
`%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup`

**Result:** Successful ✅

**Technical Finding:** Windows automatically executes all entries within the Startup directory when `explorer.exe` initializes the user session. This provides a simple user-level persistence mechanism without requiring elevated privileges.

**Limitations:** * Easily visible and disable-able via Task Manager
* Leaves a clear file artifact on disk
* Limited to the current user context
* Frequently flagged by antivirus solutions

---

## Step 2: Registry Run Key Persistence

**Hypothesis:** Adding an executable path to Windows Registry Run keys will trigger execution at user logon.

**Action:** Inserted payload path into:  
`HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run`

**Result:** Successful ✅

**Technical Finding:** The Windows logon process evaluates Run keys and executes listed binaries. With administrative privileges, persistence can be elevated to system-wide using `HKEY_LOCAL_MACHINE`.

**Limitations:** * Detectable via Task Manager and registry inspection
* Leaves explicit forensic evidence (file path)
* Monitored closely by modern antivirus solutions

---

## Step 3: WMI Event Subscription Persistence

**Hypothesis:** WMI event subscriptions can trigger payload execution based on system events without relying on traditional startup mechanisms.

**Action:** Implemented a persistence chain using:
1.  **Event Filter**
2.  **Event Consumer**
3.  **Filter-to-Consumer Binding**

**Result:** Successful ✅

**Technical Finding:** WMI persistence enables near-fileless execution by embedding logic within the Windows Management Instrumentation subsystem. Payloads can be triggered by custom system events (e.g., idle state, time-based triggers).

**Limitations:** * Typically requires administrative privileges
* Detectable through advanced forensic analysis of WMI repositories
* Higher implementation complexity

---

## Step 4: DLL Hijacking (Spotify Case Study)

**Hypothesis:** Placing a crafted DLL in an application's directory can override system DLL loading order and enable code execution.

**Action:** * Identified that `Spotify.exe` loads `version.dll` via relative path.
* Created a proxy DLL using **C++** (function forwarding technique).
* Placed the malicious DLL alongside the executable.

**Result:** Successful ✅

**Technical Finding:** Due to the default Windows DLL search order, applications that do not enforce absolute paths are vulnerable to DLL hijacking. The proxy DLL forwards legitimate function calls while executing arbitrary code in parallel.

> **Key Insight:** This method enables stealthy execution and can be extended to fileless payload delivery using shellcode loaders.

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

* **Use Absolute Paths:** Always resolve system DLLs using `GetSystemDirectory`.
* **Monitor Startup Entries:** Regularly inspect Startup folders and Run keys.
* **WMI Auditing:** Query WMI subscriptions via PowerShell (`Get-WmiObject` / `Get-CimInstance`).
* **Endpoint Protection:** Use up-to-date antivirus and behavioral detection systems.

---

## ⚠️ Legal Notice

This software is provided for **educational, ethical research, and awareness purposes only**. Understanding how Windows security architecture works is vital for developing more resilient software. The author assumes no responsibility for misuse.

## 📄 License

**MIT License** — Research, learn, and secure.

## 🧠 About

Created by **Yağız Atalay** under the **Khaelita** security research label. A study on Windows Internals, persistence techniques, and real-world detection surfaces.
