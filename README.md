# Chat Application using Semaphores

## 📌 Project Overview
This project implements a **Chat Application** using **semaphores** and **shared memory** for inter-process communication (IPC) in a Linux environment. The application enables two users (processes) to exchange messages while ensuring **proper synchronization** and **avoiding race conditions** using semaphores.

## 🛠 Features
- **Semaphore-based synchronization** to prevent data inconsistency.
- **Shared memory message passing** between two users.
- **Signal-based communication** using `SIGUSR1` and `SIGUSR2`.
- **Timeout mechanism** to handle message delays and resending.
- **Concurrency management** to ensure orderly message exchange.

## 🏗 Implementation Details
### 1️⃣ Technologies Used
- **Programming Language:** C
- **System Calls Used:** `shmget`, `shmat`, `semget`, `semctl`, `semop`, `signal`, `kill`, `fgets`, `shmdt`

### 2️⃣ Synchronization Mechanism
- **Semaphores** are used to **control access** to the shared memory, ensuring only one user can read/write at a time.
- **Shared Memory (IPC)** is used as a message buffer for inter-process communication.
- **Signals (SIGUSR1 & SIGUSR2)** trigger message reception and acknowledgment between users.
- **Timeout of 6 seconds** prevents indefinite waiting.

## 📌 How It Works
### User 1:
1. Writes a message in shared memory.
2. Sends a signal to **User 2**.
3. Waits for acknowledgment from **User 2** (or times out).

### User 2:
1. Waits for a message from **User 1**.
2. Reads and acknowledges the message.
3. Sends a response back using the same mechanism.

## 🚀 Installation & Usage
### 1️⃣ Clone the Repository
```bash
git clone https://github.com/huzaifafaran/Chat-Application-with-Semaphores.git
cd Chat-Application-with-Semaphores/Output
```

### 3️⃣ Run the Application
#### Open two terminal windows:
**In Terminal 1 (User 1):**
```bash
./u1
```
**In Terminal 2 (User 2):**
```bash
./u2
```

### 4️⃣ Start Chatting!
- Type as many messages in **User 1** terminal and press Enter.
- The messages will be received in **User 2** terminal.
- User 2 can reply in the same way.

## ⚠ Error Handling
- **If the other user is not available**, the timeout mechanism will trigger and retry.
- **If semaphores fail to initialize**, the program will exit with an error message.

## 📌 Real-World Analogy
This chat application works like a **postal system**:
- **Users** → Senders & Receivers.
- **Shared Memory** → Post Office.
- **Semaphores** → Postal Workers ensuring order.

## 📜 License
This project is open-source under the **MIT License**.

## 💡 Future Enhancements
- Add support for **multiple users** using message queues.
- Implement **GUI-based chat** using GTK or Qt.
- Enhance security with **encryption** for message exchange.

---
### ✨ Authors
- **Huzaifa Faran (22K-5197)**
- **Munawar Shereen (22K-4809)**
