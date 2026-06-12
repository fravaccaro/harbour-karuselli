---
layout: default
title: Using Karuselli
nav_order: 2
permalink: /docs/guide
---

# Using Karuselli

Karuselli edits the Sailfish OS partner space configuration stored in dconf (`/desktop/lipstick-jolla-home/partnerspace/`).

## Enable the partner space

1. Open **Karuselli**.
2. Toggle **Partner space** on from the pulley menu.
3. Tap **Apply**.
4. Restart the Home screen from the pulley menu when prompted — this is required the first time you enable the partner space.

Changing the app list later does **not** require another restart.

## Choose apps

1. With the partner space enabled, tap **Add app** (or use the pulley menu).
2. Search or scroll the list of installed apps.
3. Tap an app to add it. You can select up to **five** apps.

## Reorder and remove

- **Reorder:** long-press the drag handle on a row, then drag up or down.
- **Remove:** swipe the row and confirm the remorse timer.

Tap **Apply** to save changes.

## Cover page

The active cover shows whether the partner space is enabled and lists your selected apps.

## Native apps and transparency

Some native apps show a transparent background in the partner space. This is a known Sailfish OS issue, not a Karuselli bug. To improve appearance, install the Patchmanager patch **“Blur Partnerspace Background”**.

## Backup

The desktop file declares `X-HarbourBackup` for the partner space dconf path, so Harbour Backup can include your Karuselli layout when backing up device settings.
