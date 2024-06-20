
# Railway-management

University project for the "Sistemi Operativi" course of the bacheleor's degree in Computer Science at the University of Florence. 

It consists of the simulation of a system of trains that need authorisation to move in order to fulfil their mission.

The main objective of this project was to learn about Linux process management and IPC.

## Compilation

The following shell commands are required to compile the program
```
make setup
make all
make clean
```

## Execution

There are two possible modes of execution: ETCS1 and ETCS2.
To execute the **ERTMS** program, you must pass the desidered mode and the map as arguments.

### Examples

```
<full path>/ERTMS ETCS1 MAPPA2
```

```
<full path>/ERTMS ETCS2 MAPPA2
<full path>/ERTMS ETCS2 RBC MAPPA2
```

