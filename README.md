# ⚠️ DEPRECATED / NO LONGER MAINTAINED ⚠️

**This repository is no longer maintained or updated.**

The code here is an old PicoC implementation for the Loxone Miniserver to control Philips Hue scenes / groups via direct HTTP requests to the Hue Bridge.

**I have switched to Home Assistant** and now control all Hue lights/scenes through the official Home Assistant integration. It's much more stable, easier to extend, and actively maintained by a large community.

### Alternatives / Recommended approach in 2026

- For Loxone → Home Assistant communication:
  - First choice: the **PyLoxone** custom integration (available via HACS) – allows direct integration of many Loxone entities as sensors/switches in HA.
  - MQTT integration (very flexible and reliable).
  - The **Loxone Miniserver** add-on (if available in your HA setup).
  - RESTful / Webhook methods in HA (simple for one-way commands).
- Use **Home Assistant** + the official **Philips Hue** integration (automatic discovery).

The code remains here as a historical reference / for anyone who wants to fork it, but **I will no longer respond to issues, merge PRs, or fix bugs**.

Thanks to everyone who has used this project!
If you need help with Hue in Home Assistant → feel free to ask in the Home Assistant community (forum, Discord, or Reddit r/homeassistant).

Last update: February 2026

# Loxone-Hue-Scene-Controller

This repository contains a Pico C script to integrate Philips Hue scenes with a Loxone smart home system. Based on the original work by [Marcel Schreiner](https://github.com/marcelschreiner/loxone-hue-picoc).

## Documentation

Visit the [full documentation](https://alexkwarts.github.io/loxone-hue-scene-controller/) for detailed setup instructions, including images and step-by-step guides.

## Overview

The script bridges your Loxone Miniserver and Philips Hue lights, allowing Loxone inputs (e.g., buttons or sensors) to control pre-set Hue scenes (like "Relax" or "Bright").

## Files

- `hue_scenes_script.c`: The Pico C script for Loxone integration.
- `/docs`: Contains the HTML documentation served via GitHub Pages.
