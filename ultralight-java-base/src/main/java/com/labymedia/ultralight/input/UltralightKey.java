/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 - 2021 LabyMedia and contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

package com.labymedia.ultralight.input;

import com.labymedia.ultralight.annotation.NativeType;

import java.lang.annotation.Native;

@NativeType("const int")
public enum UltralightKey {
    /**
     * (08) BACKSPACE key
     */
    BACK(0x08),

    /**
     * (09) TAB key
     */
    TAB(0x09),

    /**
     * (0C) CLEAR key
     */
    CLEAR(0x0C),

    /**
     * (0D)
     */
    RETURN(0x0D),

    /**
     * (10) SHIFT key
     */
    SHIFT(0x10),

    /**
     * (11) CTRL key
     */
    CONTROL(0x11),

    /**
     * (12) ALT key
     */
    MENU(0x12),

    /**
     * (13) PAUSE key
     */
    PAUSE(0x13),

    /**
     * (14) CAPS LOCK key
     */
    CAPITAL(0x14),

    /**
     * (15) Input Method Editor (IME) Kana mode
     */
    KANA(0x15),

    /**
     * (15) IME Hanguel mode (maintained for compatibility), use HANGUL)
     * <p>
     * (15) IME Hangul mode
     */
    HANGUL(0x15),

    /**
     * (17) IME Junja mode
     */
    JUNJA(0x17),

    /**
     * (18) IME final mode
     */
    FINAL(0x18),

    /**
     * (19) IME Hanja mode
     */
    HANJA(0x19),

    /**
     * (19) IME Kanji mode
     */
    KANJI(0x19),

    /**
     * (1B) ESC key
     */
    ESCAPE(0x1B),

    /**
     * (1C) IME convert
     */
    CONVERT(0x1C),

    /**
     * (1D) IME nonconvert
     */
    NONCONVERT(0x1D),

    /**
     * (1E) IME accept
     */
    ACCEPT(0x1E),

    /**
     * (1F) IME mode change request
     */
    MODECHANGE(0x1F),

    /**
     * (20) SPACEBAR
     */
    SPACE(0x20),

    /**
     * (21) PAGE UP key
     */
    PRIOR(0x21),

    /**
     * (22) PAGE DOWN key
     */
    NEXT(0x22),

    /**
     * (23) END key
     */
    END(0x23),

    /**
     * (24) HOME key
     */
    HOME(0x24),

    /**
     * (25) LEFT ARROW key
     */
    LEFT(0x25),

    /**
     * (26) UP ARROW key
     */
    UP(0x26),

    /**
     * (27) RIGHT ARROW key
     */
    RIGHT(0x27),

    /**
     * (28) DOWN ARROW key
     */
    DOWN(0x28),

    /**
     * (29) SELECT key
     */
    SELECT(0x29),

    /**
     * (2A) PRINT key
     */
    PRINT(0x2A),

    /**
     * (2B) EXECUTE key
     */
    EXECUTE(0x2B),

    /**
     * (2C) PRINT SCREEN key
     */
    SNAPSHOT(0x2C),

    /**
     * (2D) INS key
     */
    INSERT(0x2D),

    /**
     * (2E) DEL key
     */
    DELETE(0x2E),

    /**
     * (2F) HELP key
     */
    HELP(0x2F),

    /**
     * 0 key
     */
    NUM_0(0x30),

    /**
     * 1 key
     */
    NUM_1(0x31),

    /**
     * 2 key
     */
    NUM_2(0x32),

    /**
     * 3 key
     */
    NUM_3(0x33),

    /**
     * 4 key
     */
    NUM_4(0x34),

    /**
     * 5 key),
     */
    NUM_5(0x35),

    /**
     * 6 key
     */
    NUM_6(0x36),

    /**
     * 7 key
     */
    NUM_7(0x37),

    /**
     * 8 key
     */
    NUM_8(0x38),

    /**
     * 9 key
     */
    NUM_9(0x39),

    /**
     * A key
     */
    A(0x41),

    /**
     * B key
     */
    B(0x42),

    /**
     * C key
     */
    C(0x43),

    /**
     * D key
     */
    D(0x44),

    /**
     * E key
     */
    E(0x45),

    /**
     * F key
     */
    F(0x46),

    /**
     * G key
     */
    G(0x47),

    /**
     * H key
     */
    H(0x48),

    /**
     * I key
     */
    I(0x49),

    /**
     * J key
     */
    J(0x4A),

    /**
     * K key
     */
    K(0x4B),

    /**
     * L key
     */
    L(0x4C),

    /**
     * M key
     */
    M(0x4D),

    /**
     * N key
     */
    N(0x4E),

    /**
     * O key
     */
    O(0x4F),

    /**
     * P key
     */
    P(0x50),

    /**
     * Q key
     */
    Q(0x51),

    /**
     * R key
     */
    R(0x52),

    /**
     * S key
     */
    S(0x53),

    /**
     * T key
     */
    T(0x54),

    /**
     * U key
     */
    U(0x55),

    /**
     * V key
     */
    V(0x56),

    /**
     * W key
     */
    W(0x57),

    /**
     * X key
     */
    X(0x58),

    /**
     * Y key
     */
    Y(0x59),

    /**
     * Z key
     */
    Z(0x5A),

    /**
     * (5B) Left Windows key (Microsoft Natural keyboard)
     */
    LWIN(0x5B),

    /**
     * (5C) Right Windows key (Natural keyboard)
     */
    RWIN(0x5C),

    /**
     * (5D) Applications key (Natural keyboard)
     */
    APPS(0x5D),

    /**
     * (5F) Computer Sleep key
     */
    SLEEP(0x5F),

    /**
     * (60) Numeric keypad 0 key
     */
    NUMPAD0(0x60),

    /**
     * (61) Numeric keypad 1 key
     */
    NUMPAD1(0x61),

    /**
     * (62) Numeric keypad 2 key
     */
    NUMPAD2(0x62),

    /**
     * (63) Numeric keypad 3 key
     */
    NUMPAD3(0x63),

    /**
     * (64) Numeric keypad 4 key
     */
    NUMPAD4(0x64),

    /**
     * (65) Numeric keypad 5 key
     */
    NUMPAD5(0x65),

    /**
     * (66) Numeric keypad 6 key
     */
    NUMPAD6(0x66),

    /**
     * (67) Numeric keypad 7 key
     */
    NUMPAD7(0x67),

    /**
     * (68) Numeric keypad 8 key
     */
    NUMPAD8(0x68),

    /**
     * (69) Numeric keypad 9 key
     */
    NUMPAD9(0x69),

    /**
     * (6A) Multiply key
     */
    MULTIPLY(0x6A),

    /**
     * (6B) Add key
     */
    ADD(0x6B),

    /**
     * (6C) Separator key
     */
    SEPARATOR(0x6C),

    /**
     * (6D) Subtract key
     */
    SUBTRACT(0x6D),

    /**
     * (6E) Decimal key
     */
    DECIMAL(0x6E),

    /**
     * (6F) Divide key
     */
    DIVIDE(0x6F),

    /**
     * (70) F1 key
     */
    F1(0x70),

    /**
     * (71) F2 key
     */
    F2(0x71),

    /**
     * (72) F3 key
     */
    F3(0x72),

    /**
     * (73) F4 key
     */
    F4(0x73),

    /**
     * (74) F5 key
     */
    F5(0x74),

    /**
     * (75) F6 key
     */
    F6(0x75),

    /**
     * (76) F7 key
     */
    F7(0x76),

    /**
     * (77) F8 key
     */
    F8(0x77),

    /**
     * (78) F9 key
     */
    F9(0x78),

    /**
     * (79) F10 key
     */
    F10(0x79),

    /**
     * (7A) F11 key
     */
    F11(0x7A),

    /**
     * (7B) F12 key
     */
    F12(0x7B),

    /**
     * (7C) F13 key
     */
    F13(0x7C),

    /**
     * (7D) F14 key
     */
    F14(0x7D),

    /**
     * (7E) F15 key
     */
    F15(0x7E),

    /**
     * (7F) F16 key
     */
    F16(0x7F),

    /**
     * (80H) F17 key
     */
    F17(0x80),

    /**
     * (81H) F18 key
     */
    F18(0x81),

    /**
     * (82H) F19 key
     */
    F19(0x82),

    /**
     * (83H) F20 key
     */
    F20(0x83),

    /**
     * (84H) F21 key
     */
    F21(0x84),

    /**
     * (85H) F22 key
     */
    F22(0x85),

    /**
     * (86H) F23 key
     */
    F23(0x86),

    /**
     * (87H) F24 key
     */
    F24(0x87),

    /**
     * (90) NUM LOCK key
     */
    NUMLOCK(0x90),

    /**
     * (91) SCROLL LOCK key
     */
    SCROLL(0x91),

    /**
     * (A0) Left SHIFT key
     */
    LSHIFT(0xA0),

    /**
     * (A1) Right SHIFT key
     */
    RSHIFT(0xA1),

    /**
     * (A2) Left CONTROL key
     */
    LCONTROL(0xA2),

    /**
     * (A3) Right CONTROL key
     */
    RCONTROL(0xA3),

    /**
     * (A4) Left MENU key
     */
    LMENU(0xA4),

    /**
     * (A5) Right MENU key
     */
    RMENU(0xA5),

    /**
     * (A6) Windows 2000/XP: Browser Back key
     */
    BROWSER_BACK(0xA6),

    /**
     * (A7) Windows 2000/XP: Browser Forward key
     */
    BROWSER_FORWARD(0xA7),

    /**
     * (A8) Windows 2000/XP: Browser Refresh key
     */
    BROWSER_REFRESH(0xA8),

    /**
     * (A9) Windows 2000/XP: Browser Stop key
     */
    BROWSER_STOP(0xA9),

    /**
     * (AA) Windows 2000/XP: Browser Search key
     */
    BROWSER_SEARCH(0xAA),

    /**
     * (AB) Windows 2000/XP: Browser Favorites key
     */
    BROWSER_FAVORITES(0xAB),

    /**
     * (AC) Windows 2000/XP: Browser Start and Home key
     */
    BROWSER_HOME(0xAC),

    /**
     * (AD) Windows 2000/XP: Volume Mute key
     */
    VOLUME_MUTE(0xAD),

    /**
     * (AE) Windows 2000/XP: Volume Down key
     */
    VOLUME_DOWN(0xAE),

    /**
     * (AF) Windows 2000/XP: Volume Up key
     */
    VOLUME_UP(0xAF),

    /**
     * (B0) Windows 2000/XP: Next Track key
     */
    MEDIA_NEXT_TRACK(0xB0),

    /**
     * (B1) Windows 2000/XP: Previous Track key
     */
    MEDIA_PREV_TRACK(0xB1),

    /**
     * (B2) Windows 2000/XP: Stop Media key
     */
    MEDIA_STOP(0xB2),

    /**
     * (B3) Windows 2000/XP: Play/Pause Media key
     */
    MEDIA_PLAY_PAUSE(0xB3),

    /**
     * (B4) Windows 2000/XP: Start Mail key
     */
    MEDIA_LAUNCH_MAIL(0xB4),

    /**
     * (B5) Windows 2000/XP: Select Media key
     */
    MEDIA_LAUNCH_MEDIA_SELECT(0xB5),

    /**
     * (B6) Windows 2000/XP: Start Application 1 key
     */
    MEDIA_LAUNCH_APP1(0xB6),

    /**
     * (B7) Windows 2000/XP: Start Application 2 key
     */
    MEDIA_LAUNCH_APP2(0xB7),

    /**
     * (BA) Used for miscellaneous characters),
     * it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '),:' key
     */
    OEM_1(0xBA),

    /**
     * (BB) Windows 2000/XP: For any country/region, the '+' key
     */
    OEM_PLUS(0xBB),

    /**
     * (BC) Windows 2000/XP: For any country/region, the ',' key
     */
    OEM_COMMA(0xBC),

    /**
     * (BD) Windows 2000/XP: For any country/region, the '-' key
     */
    OEM_MINUS(0xBD),

    /**
     * (BE) Windows 2000/XP: For any country/region, the '.' key
     */
    OEM_PERIOD(0xBE),

    /**
     * (BF) Used for miscellaneous characters),
     * it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '/?' key
     */
    OEM_2(0xBF),

    /**
     * (C0) Used for miscellaneous characters),
     * it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '`~' key
     */
    OEM_3(0xC0),

    /**
     * (DB) Used for miscellaneous characters),
     * it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '[{' key
     */
    OEM_4(0xDB),

    /**
     * (DC) Used for miscellaneous characters),
     * it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '\|' key
     */
    OEM_5(0xDC),

    /**
     * (DD) Used for miscellaneous characters),
     * it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ']}' key
     */
    OEM_6(0xDD),

    /**
     * (DE) Used for miscellaneous characters),
     * it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
     */
    OEM_7(0xDE),

    /**
     * (DF) Used for miscellaneous characters), it can vary by keyboard.
     */
    OEM_8(0xDF),

    /**
     * (E2) Windows 2000/XP: Either the angle bracket key or the backslash key on the RT 102-key keyboard
     */
    OEM_102(0xE2),

    /**
     * (E5) Windows 95/98/Me, Windows NT 4.0, Windows 2000/XP: IME PROCESS key
     */
    PROCESSKEY(0xE5),

    /**
     * (E7) Windows 2000/XP: Used to pass Unicode characters as if they were keystrokes.
     * The PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods.
     * For more information, see Remark in KEYBDINPUT,SendInput, WM_KEYDOWN, and WM_KEYUP
     */
    PACKET(0xE7),

    /**
     * (F6) Attn key
     */
    ATTN(0xF6),

    /**
     * (F7) CrSel key
     */
    CRSEL(0xF7),

    /**
     * (F8) ExSel key
     */
    EXSEL(0xF8),

    /**
     * (F9) Erase EOF key
     */
    EREOF(0xF9),

    /**
     * (FA) Play key
     */
    PLAY(0xFA),

    /**
     * (FB) Zoom key
     */
    ZOOM(0xFB),

    /**
     * (FC) Reserved for future use
     */
    NONAME(0xFC),

    /**
     * (FD) PA1 key
     */
    PA1(0xFD),

    /**
     * (FE) Clear key
     */
    OEM_CLEAR(0xFE),

    /**
     * Unknown key
     */
    UNKNOWN(0);

    /**
     * The id that is
     */
    @Native
    private final int id;

    UltralightKey(int id) {
        this.id = id;
    }
}
