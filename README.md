 


# charleX OS

## Logo

```
                  %%  %%                      %%  %%
                 %%      %%                  %%      %%
                 %%        %%              %%        %%
                 %%          %%%%%%%%%%%%%%          %%
                 %%                                  %%
                 %%                                  %%
               %%        %%%%              %%%%        %%
             %%          %%%%%%          %%%%%%          %%
             %%                                          %%
             %%                  %%%%%%                  %%
             %%            %%      %%      %%            %%
               %%            %%%%%%%%%%%%%%            %%
                %%                                    %%
                   %%                              %%
                     %%%%%%%%              %%%%%%%%
                             %%%%%%%%%%%%%%
```

## Description

CharleX OS is a minimal, open-source operating system kernel written primarily in C with assembly components. It provides a basic terminal interface in VGA text mode, supporting keyboard input for a simple command-line experience. This project serves as an educational platform for understanding low-level OS development, inspired by early Unix-like systems such as Linux and FreeBSD.

The kernel includes features like:
- VGA text mode initialization and output
- Keyboard input handling with support for basic keys (enter, backspace, arrows, etc.)
- A simple shell-like prompt for user interaction
- Bootloader integration with GRUB

## Screenshots

![GRUB Bootloader](screenshots/charleXGrub.png)

![Loading and Logo](screenshots/charleXLoadingAndLogo.png)

![Command Prompt](screenshots/charleXPrompt.png)

## Building and Running

### Prerequisites
- GCC compiler
- NASM assembler
- GRUB tools
- xorriso for ISO creation

### On Debian/Ubuntu
Install dependencies:
```bash
make deps
```

Build the ISO:
```bash
make mkiso
```

Run the OS using a virtual machine or boot from the generated `charleX.iso`.


## Contributing

CharleX OS is an open-source project, and contributions are welcome! Feel free to:
- Report issues
- Submit pull requests for bug fixes or new features
- Improve documentation
- Star the repository to show support

## License

This project is licensed under the terms specified in the `LICENSE` file.
