# Protocol-5-Go-Back-N

This project implements Protocol-5, which allows the transmission of multiple frames without acknowledgment , and goes back to resend buffered unacknowledged frames in order.

## Features
- Implements Go-Back-N ARQ protocol.
- Allows transmission of multiple frames in sequence before requiring acknowledgment.
- Error handling and retransmission strategies are implemented.

## How to Compile and Run

To compile and run the program, you can use the following command:
```powershell
cd "<path-to-project-folder>" ; if ($?) { g++ Main.cpp protocol.cpp print_func.cpp -o program } ; if ($?) { .\program }
   ```
## Debugging and Understanding the Code

To assist with debugging and provide a better understanding of the code, the following files have been configured in the `.vscode` folder:

## launch.json

This file configures the debugging settings for the program. It allows you to:
- Set breakpoints
- Step through the code
- Inspect variables

## tasks.json

This file automates tasks such as building the program, making it easier to compile without manually typing commands.
