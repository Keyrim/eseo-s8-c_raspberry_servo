EXEC = projet_trop_style.elf

# Répertoire d'installation des outils de développement
# pour Raspberry Pi (à adapter, si besoin).
# exemple : RASPBERRY_TOOLS = /home/matth/Logiciels/RaspberryPi/Tools/

# Compilateur croisé à utiliser (à adapter)
RASPBERRY_GCC = arm-linux-gnueabihf-gcc
# exemple : RASPBERRY_GCC = $(RASPBERRY_TOOLS)/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc

# Sysroot (à adapter)
#RASPBERRY_SYSROOT = /<path>/<to>/<raspberry>/<rootfs>/
# exemple : RASPBERRY_SYSROOT = $(RASPBERRY_TOOLS)/rootfs_bplus/

# Compilateur.
CC = $(RASPBERRY_GCC)
CCFLAGS += -Wall -pedantic

# Linker
LD = $(RASPBERRY_GCC)
LDFLAGS += -lgpiod -lpthread -lrt


#Organisation des sources
SRC = 	$(wildcard *.c) 

OBJ = 	$(SRC:.c=.o)

#Regles de compilations
all: $(EXEC)

$(EXEC): $(OBJ)
	$(LD) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@
	
.PHONY: clean cp_app

#Clean Project
clean:
	$(RM) *.o $(EXEC)

#Compile et lance l'applicatif
run: $(EXEC)
	./$(EXEC)