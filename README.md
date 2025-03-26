# Promo System for AzerothCore

### RU locales [README_RU](https://github.com/levrules/mod-promo-codes/blob/main/README_RU.md)

## Description

This module adds a promo code system for the AzerothCore server (World of Warcraft 3.3.5a). Players can enter special codes via a command in the chat and receive various rewards, including items, gold, honor points, arena points, faction change and other bonuses.

## Features
- Activation of promo codes via the command `/promo <code>`.
- Issuance of various rewards depending on the type of promo code.
- Restrictions on the use of promo codes (uniqueness per character or account).
- Automatic reduction of the number of available promo codes.
- Checking the time limits of promo codes (start and end dates).

> [!IMPORTANT]
## Installation
1. Clone this repository:
```bash
cd path/to/azerothcore/modules
git clone https://github.com/levrules/mod-promo-codes.git
```
2. Re-run cmake and run a clean build of AzerothCore.
3. Restart the server to apply the changes.
4. ClientSide must be installed in the patch
5. Include the file `ClientSide/Interface/FrameXML/Custom_PromoCode/Custom_PromoCode.xml` in the file `Interface/FrameXML/FrameXML.toc`

> [!CAUTION]
## Please note!
- To display information correctly, change the text in the `acore_string` table.

- Display in the frame occurs only by the PROMO keyword

## Usage
Players can redeem promo codes by using the following command in chat:
```
/promo <code>
```
Example:
```
/promo FREEMOUNT2024
```
## Reward Types (`promo.type`)

| Type | Description |
|-----|----------|
| `0` | Item Granted |
| `1` | Spell Learned |
| `2` | Gold issue (not bronze) |
| `3` | Honor points accrual |
| `4` | Arena points accrual |
| `5` | Faction change |
| `6` | Name change |
| `7` | Race change |

## Example of adding a promo code to the database
```sql
INSERT INTO promo (code, type, value, amount, count, uniqueCharacters, uniqueAccount, enable, start_time, end_time)
VALUES ('FREEMOUNT2024', 0, 49908, 20, 100, 1, 1, 1, '2025-01-01 00:00:00', '2025-12-31 23:59:59');
```
This promo code will give the player the item with the ID `49908` (Ancient Saronite), available 100 times, with a limit of 1 time per character and account, active throughout 2025.

## Authors
- Developer: *Moloko*
- Contacts: *[GitHub](https://github.com/levrules)*

- Idea submitted by: *laenholdqqs*
- Contacts: *[GitHub](https://github.com/laenholdqqs)*