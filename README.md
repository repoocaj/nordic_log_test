# Introduction

Test project to a demonstrate question on logging with Nordic SDK v15.3.0.

The project started out as a framework for a DVT test of a flash chip on a
custom board.  However, when the log output wasn't as I expected it to be, I
converted it to a test program to illustrate my question.

This example is show run on a nRF52840-DK board (PCA10056) with the log output
viewed by JLinkRTTClient.

The version of SEGGER tools is SEGGER J-Link V6.48b.

## Expected Output

What I expected to see when the DK is reset is the following output in the
JLinkRTTClient:

```
Log Test v1.0.0
<info> app: LEDs initialized
<info> app: LEDs off
<info> app: Mode 0
<info> app: Processed 3 entries
```

One second pause, LED1 on, LED2 off

```
<info> app: Erase Flash
<info> app: Mode 1
<info> app: Processed 1 entries
```

One second pause, LED1 off, LED2 on

```
<info> app: Verify Erase
<info> app: Mode 2
<info> app: Processed 1 entries
```

One second pause, LED1 on, LED2 off

```

<info> app: Write Flash
<info> app: Mode 3
<info> app: Processed 1 entries
```

One second pause, LED1 off, LED2 on

```

<info> app: Verify Write
<info> app: Mode 4
<info> app: Processed 1 entries
```

One second pause, LED1 on, LED2 off

```
<info> app: Result: Passed
Done
```

## Actual Output

One second pause, LED1 on, LED2 off

One second pause, LED1 off, LED2 on

One second pause, LED1 on, LED2 off

One second pause, LED1 off, LED2 on

One second pause, LED1 on, LED2 off

Followed by the block of data in the JLinkRTTClient:

```
Log Test v1.0.0
<info> app: LEDs initialized
<info> app: LEDs off
<info> app: Mode 0
<info> app: Processed 3 entries
<info> app: Erase Flash
<info> app: Mode 1
<info> app: Processed 1 entries
<info> app: Verify Erase
<info> app: Mode 2
<info> app: Processed 1 entries
<info> app: Write Flash
<info> app: Mode 3
<info> app: Processed 1 entries
<info> app: Verify Write
<info> app: Mode 4
<info> app: Processed 1 entries
<info> app: Result: Passed
Done

