# What's this?
This repo contains the source for a cryptography/hacking treasure hunt I made for my friend in 2025.
It's written in C and contains some steganographic trickery with file magic numbers and other quirky behavious.

# What's in it?
The repo contains the source code, makefile and all asset files to compile the executable containing the puzzle. There is a design doc (HUNT_DESIGN.txt) I wrote during the period I was working on this. The design doc implemented most of the ideas in the final project, with small deviations.
There is a simple obfuscator and deobfuscator pair which can mangle any data at the byte level. I wrote it to hide the data and make it harder to parse any signatures.
The final solution revealed some gift codes (redacted) as well as the entire source code of the puzzle contained within itself.

# How does it work?
The original hunt executable was provided with an audio recording containing some silly plot about conspiracies and a DTMF encoded byte string at the end. The puzzle/executable itself is in three layers, with each building on top of the previous ones' principles. As the hunter progresses through the hunt more of the lore is revealed about wormholes and portals. This is loosely referencing the game distribution platform Steam, for which the solution provided gift codes.

# How do I build it?
<font color="red">WARNING! This project creates an executable with embedded obfuscated binary data. This can trigger anti-malware and security software</font>

The project requires GCC, binutils and 7zip to build. Running `make` will build both a linux (`m.x`) and windows (`layer1.exe`) version, but the windows build might fail if the host does not have `x86_64-w64-mingw32-gcc`. The `build` directory will contain all the intermediary build files that are created and packed into the layers during the compilation process. It was done this way since the puzzle is assembled "backwards" from the end. Running `make clean` will purge the contents of the build directory and will delete the de/obfuscator and hunt executables.

## Linux
The linux executable can be built by running `make pack_src m.x`.

## Windows
The Windows executable can be built by running `make pack_src layer1.exe`

## De/Obfuscator
The obfuscator/deobfuscator pair can be built by running `make obfus.x`

# De/Obfuscator usage
The de/obfuscator takes a file and depending on input arguments will create an obfuscated or deobfuscated output. It actually has more functionality than was used in the hunt itself and can be used standalone.

<font color="red">WARNING! Obfuscated files created by the obfucator may be interpreted as malicious files by security software! Do not run the obfuscator on any machine with any enterprise security software</font>

The usage is:
`obfus.x INPUT_FILENAME OUTPUT_FILENAME PADDING SCHEME EXTRA`:

* `INPUT_FILENAME`: self explanatory
* `OUTPUT_FILENAME`: self expanatory
* `PADDING`: `n, r, o` Pad the output data where every odd byte is `r`andom or `z`ero. Or do`n`'t pad the output
* `SCHEME`: `o, x` Every data byte can be `o`ffset or `x`or'ed.
* `EXTRA`: If `SCHEME` is `o`ffset, then two values are expected and will be added to every data byte in alternating pairs (padding is ignored). If `SCHEME` is `x`or, then one value is expected and will be XOR'd with every data byte

# Intended hunt solution
Running the executable in a shell will prompt for an entry.

Providing the byte string decoded from the DTMF will reveal a limerick containing a hint on what to do. The hunter is supposed to view the executable in a hex editor and find an instruction in plain text calling to flip a specific byte from A5 to 5A.

Rerunning the executable after the modification will dump some lore text on screen as well as start printing a hex dump of a file. The hunter does not need to wait for the printout to complete as the contents have already been printed into a file called layer2.txt

Copying the hex stream back into a hex editor and saving the file as a JPG shows an image containing a hint - "Delete the image, keep the remainder". This file is actually two files stacked on top of each other - a JPG image and a ZIP archive.

Some archive applications, such as 7zip will actually ignore the JPG portion and can open the file as a zip archive successfully. Alternatively the hunter can look for the JPEG EOI (FF F9) and see the next magic number for a ZIP archive (50 4B 03). Opening this archive reveals the contents: another archive called layer3.zip and a bit of lore in a file called part2.txt

Inside layer3.zip are 3 files: 1.zip, 2.jpg, 4.jpg. This layer has two diversions - a literal red herring and implication of a fourth file called "3", which does no exist. The three files are:
* 1.zip is actually two files stacked on top again. This time an archive and an image. Spliting this into two files will reveal a zip archive and a jpg image. The jpg is an image of a key with the text saying "VIGENERE'S ALPHABET: QWERTYUIOPASDFGHJKLZXCVBNM". The archive contains a file called ciphertext.txt. Inside is a vigenere's encoded string, which if decoded using the key "KEY" and alphabet from before will reveal the message "A key needs a lock. Find one" followed by a long string.
* 2.jpg is an image of a man fishing. The hunter may notice that this file is fairly large at 6MB. It is a decoy. This is actually two JPG images stacked on top of each other. The fisherman image is large and after it is a JPG image of a red herring with a speech bubble "I'm here to distract you"
* 4.jpg is an image of a man recursively staring at himself in his own reflection. The hint is supposed to make the hunter think of recursion as a key principle in this puzzle. Viewing the file in a hex editor will reveal that past the JPG EOI is a string of ascii characters. This is actually a string of bytes that must be interpreted as hex to reveal another hex string which finally reveals another file. This file's magic numbers actually show it to be a 7zip archive. Inside it is a file called sequence_keys.txt

Attempting to extract this final 7zip will prompt for a password. Provided the hunter has decoded the string from 1.zip they can successfully extract the final message, congratulating the hunter, giving them their gift codes (which have been redacted for this repo), and also revealing that if they change a different byte in the initial executable and re-run - it will dump the source code for this project.

# Disclaimer
No AI was used in the creation of this project
