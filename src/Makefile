CC            = gcc
CCWIN         = x86_64-w64-mingw32-gcc
PROJECT_DIR   = $(PWD)/

DEBUG ?= 0
ifeq ($(DEBUG),1)
   DEBUGARG     = -DDEBUG -ggdb
endif
OLEVEL       = -O3
USER_DEFINES =  $(OLEVEL) $(DEBUGARG)

LAY1_DIR            = $(PROJECT_DIR)layer1_parts/
BUILD_DIR           = $(PROJECT_DIR)build/
LAY1STRIPLIST       = secret.txt
LAY1TEXTLIST        = hint.txt dtmf.txt unlock.txt
LAY1DATALIST        = payload.txt
LAY1SRCZIP          = $(addprefix $(BUILD_DIR),src.zip)
LAY1PAYLOAD         = $(addprefix $(BUILD_DIR),$(LAY1DATALIST)_UNOBFUS)
LAY1TEXTOBFUS       = $(addprefix $(BUILD_DIR),$(LAY1TEXTLIST))
LAY1DATAOBFUS       = $(addprefix $(BUILD_DIR),$(LAY1DATALIST))
LAY1STRIPPED        = $(addprefix $(BUILD_DIR),$(LAY1STRIPLIST))

LAY2_DIR            = $(PROJECT_DIR)layer2_parts/
LAY2TEXTLIST        = lore.txt hint.txt
LAY2DATALIST        = image.jpg
LAY2PAYLOAD         = $(addprefix $(BUILD_DIR),l2payload.hex)
LAY2TEXT            = $(addprefix $(LAY2_DIR),$(LAY2TEXTLIST))
LAY2DATA            = $(addprefix $(LAY2_DIR),$(LAY2DATALIST))

LAY3_DIR            = $(PROJECT_DIR)layer3_parts/
LAY3TEXTLIST        = part2.txt
LAY3ROOT            = $(addprefix $(BUILD_DIR),layer3.zip)
LAY3FILE1CONTENT    = $(addprefix $(LAY3_DIR),ciphertext.txt key.jpg)
LAY3FILE1			  = $(addprefix $(BUILD_DIR),1.zip)
LAY3FILE2CONTENT    = $(addprefix $(LAY3_DIR),fishing.jpg herring.jpg)
LAY3FILE2			  = $(addprefix $(BUILD_DIR),2.jpg)
LAY3FILE3CONTENT    = $(addprefix $(LAY3_DIR),sequence_keys.txt)
LAY3FILE3			  = $(addprefix $(BUILD_DIR),3.7z)
LAY3FILE4CONTENT    = $(addprefix $(LAY3_DIR),recursion.jpg)
LAY3FILE4			  = $(addprefix $(BUILD_DIR),4.jpg)
LAY3TEXT            = $(addprefix $(LAY3_DIR),$(LAY3TEXTLIST))
LAY3PAYLOAD         = $(addprefix $(BUILD_DIR),l3payload.zip)

ZIPCMD              = zip -X
7ZIPCMD             = 7z a
7ZIPTYPEZIP         = -tzip
7ZIPTYPE7z          = -t7z
7ZIPSWENC           = -p$(7ZIPPASSWORD)
7ZIPPASSWORDFILE    = $(addprefix $(LAY3_DIR), password.txt)
7ZIPPASSWORD        = $(shell cat $(7ZIPPASSWORDFILE))

SRC                 = $(wildcard $(PROJECT_DIR)*.c)
ASM                 = $(wildcard $(PROJECT_DIR)*.S)

FFLAGS = -fdata-sections -ffunction-sections
LDFLAGS= 
LFLAGS = 
WFLAGS = -W -Wall -Wl,--gc-sections

CFLAGS = $(FFLAGS) $(LDFLAGS) $(LFLAGS) $(WFLAGS) $(USER_DEFINES)

OBFUSCATOR = obfus.x
DEOBFUSCATOR = deobf.x

.PHONY: clean

all: layer1

pack_src: $(LAY1SRCZIP)

$(LAY1SRCZIP):
	$(7ZIPCMD) $(7ZIPTYPEZIP) $(7ZIPSWENC) $(LAY1SRCZIP) *.c *.h *.S Makefile *.txt build/ layer1_parts/* layer2_parts/* layer3_parts/* ;\
	$(7ZIPCMD) $(7ZIPTYPEZIP) $(7ZIPSWENC) -sdel $(LAY1SRCZIP)_tmp $(LAY1SRCZIP) ;\
	mv $(LAY1SRCZIP)_tmp $(LAY1SRCZIP) ;\
	touch if_you_found_this_before_solving_layer3_ignore_it ;\
	$(7ZIPCMD) $(7ZIPTYPEZIP) $(LAY1SRCZIP) -sdel if_you_found_this_before_solving_layer3_ignore_it

layer1: pack_src layer2 $(LAY1PAYLOAD) m.x layer1.exe

layer2: layer3 $(LAY2PAYLOAD)

layer3: $(LAY3PAYLOAD)

$(LAY3PAYLOAD): $(LAY3ROOT) $(LAY3TEXT)
	$(7ZIPCMD) $(7ZIPTYPEZIP) $@ $(LAY3ROOT);\
	$(7ZIPCMD) $(7ZIPTYPEZIP) $@ $(LAY3TEXT)

$(LAY3ROOT): $(LAY3FILE1) $(LAY3FILE2) $(LAY3FILE4)
	$(7ZIPCMD) $(7ZIPTYPEZIP) $(LAY3ROOT) $^

$(LAY3FILE1): $(LAY3FILE1CONTENT)
	$(7ZIPCMD) $(7ZIPTYPEZIP) $(LAY3_DIR)tmp.zip $(LAY3_DIR)ciphertext.txt;\
	cat $(LAY3_DIR)tmp.zip $(LAY3_DIR)key.jpg > $@;\
	rm -rf $(LAY3_DIR)tmp.zip

$(LAY3FILE2): $(LAY3FILE2CONTENT) $(LAY3FILE3)
	cat $(LAY3_DIR)fishing.jpg $(LAY3_DIR)herring.jpg > $@

$(LAY3FILE3): $(LAY3FILE3CONTENT)
	$(7ZIPCMD) $(7ZIPTYPE7z) $(7ZIPSWENC) $@ $^

$(LAY3FILE4): $(LAY3FILE4CONTENT) $(LAY3FILE3)
	hexdump -v -e '16/1 "%02x " "\n"' $(LAY3FILE3) | hexdump -v -e '16/1 "%02x " "\n"' > $(LAY3_DIR)tmp.txt;\
	cat $(LAY3_DIR)recursion.jpg $(LAY3_DIR)tmp.txt > $@;\
	rm -rf $(LAY3_DIR)tmp.txt

$(LAY2PAYLOAD): $(LAY2DATA) $(LAY3PAYLOAD)
	cat $^ | hexdump -v -e '16/1 "%02x " "\n"' > $@

$(LAY1PAYLOAD): $(LAY2TEXT) $(LAY2PAYLOAD)
	cat $^ > $(LAY1PAYLOAD)

m.x: layer1_blob.bin main.c printlib.c obfuslib.c filereaderwriter.c
	$(CC) $(CFLAGS) layer1_blob.bin filereaderwriter.c obfuslib.c printlib.c main.c -o $@
ifeq ($(DEBUG),0)
	strip -s $@
endif

layer1.exe: layer1_blob.bin main.c printlib.c obfuslib.c filereaderwriter.c
	$(CCWIN) $(CFLAGS) layer1_blob.bin filereaderwriter.c obfuslib.c printlib.c main.c -o $@
ifeq ($(DEBUG),0)
	strip -s $@
endif

layer1_blob.bin: $(ASM) $(LAY1TEXTOBFUS) $(LAY1DATAOBFUS) $(LAY1STRIPPED) $(LAY1SRCZIP)_OBFUS
	$(CC) $(foreach f,$(LAY1STRIPLIST),-D__$(basename $(f))__='"$(BUILD_DIR)$(f)"') $(foreach f,$(LAY1TEXTLIST),-D__$(basename $(f))__='"$(BUILD_DIR)$(f)"') $(foreach f,$(LAY1DATALIST),-D__$(basename $(f))__='"$(BUILD_DIR)$(f)"'), -D__srcblock__='"$(LAY1SRCZIP)_OBFUS"' $(ASM) -c -o $@

$(LAY1TEXTOBFUS):: $(OBFUSCATOR)
$(LAY1TEXTOBFUS):: $(BUILD_DIR)%: $(LAY1_DIR)%
	./obfus.x $^ $@ r o 2 -3;
$(LAY1DATAOBFUS):: $(OBFUSCATOR)
$(LAY1DATAOBFUS):: $(BUILD_DIR)%: $(BUILD_DIR)%_UNOBFUS
	echo "Making $@ with $^";\
	./obfus.x $^ $@ n x 23;\

$(LAY1SRCZIP)_OBFUS: $(LAY1SRCZIP)
	./obfus.x $^ $@ n o 3 -2;\

$(LAY1STRIPPED): $(BUILD_DIR)%: $(LAY1_DIR)%
	cp $^ $@;\
	truncate -s -1 $@;

$(OBFUSCATOR):: $(DEOBFUSCATOR)
$(OBFUSCATOR):: obfuslib.c obfuscator.c filereaderwriter.c
	$(CC) $(CFLAGS) -o $@ $^

$(DEOBFUSCATOR): obfuslib.c deobfuscator.c filereaderwriter.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@echo "Cleaning up"; \
	rm -rf *.x *.bin *.exe build/*
