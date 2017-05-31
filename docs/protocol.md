# spp: a small printer protocol

### example:
*note: ^[ is ASCII 1B or ␛*
```
^[b;ASCII^[!b; (i/'a^[hb;eski/ ass-kee), abbreviated from ^[r;American Standard Code for Information Interchange^[!r;,␍␊
is a character encoding standard (the Internet Assigned Numbers Authority (IANA) prefers the name US-ASCII[2]).␍␊
ASCII codes represent text in computers, telecommunications equipment, and other devices.␍␊
Most modern character-encoding schemes are based on ASCII, although they support many additional characters.␍␊
```

### specification:
All communication is done in ASCII.

All printable characters sent outside of an escape will be printed by the device automatically.
The tabstop is 8 spaces by default.

The following control-block characters are recognized and interpreted:

| hex | name                | effect                                                                      |
| :-- | :------------------ | :-------------------------------------------------------------------------- |
| 05  | Enquiry (␅)         | Returns an 0x06 ACK (␆)                                                     |
| 0D  | Carriage Return (␍) | Returns print-head to start of line.                                        |
| 0A  | Line Feed (␊)       | Moves print-head down one line.                                             |
| 08  | Backspace (␈)       | Move the print-head back one space.                                         |
| 7F  | Delete (␡)          | Write-out a character using the correct ribbon (if present). No implicit ␈  |
| 1B  | Escape (␛)          | Escape a control sequence.                                                  |

Escape sequences are sent as `^[<sequence>;`; that is, `␛<sequence>;`.
Available sequences are:

| sequence      | modal? | effect                                                                                  |
| :------------ | ------ | :-------------------------------------------------------------------------------------- |
| b             | ✔      | Turns on bold mode (implemented as restrike).                                           |
| hb            | ✘      | Half-backspace, used for combining characters, e.g. the ash grapheme (æ)                |
| sp            | ✔      | Turns on superscript mode                                                               |
| sb            | ✔      | Turns on subscript mode                                                                 |
| sts$ts        | ✘      | [*configuration*] set tab stop to `$ts` spaces                                          |

Modal sequences are called as given to be turned on, and called with a prefixed `!` to be turned off, *e.g.* `^[b` and `^[!b`.