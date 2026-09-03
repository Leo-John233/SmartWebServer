# Smart Web Server Web UI

This directory contains the Vue 3 development environment for the ESP8266 interface

## Commands

- `npm run dev` starts the local MSW simulation at `http://127.0.0.1:5173`
- `npm run dev:device` proxies legacy requests to the configured ESP device
- `npm run test` runs protocol and component tests
- `npm run test:e2e` builds and verifies the production preview with Chromium
- `npm run build` creates the production bundle in `dist/`
- `npm run build:embed` creates the `/ui/`-based bundle used by the embed pipeline
- `npm run preview` serves the latest production build at `http://127.0.0.1:4173`

## Device proxy

Copy `.env.example` to `.env.local` and change `VITE_DEVICE_URL` when the ESP is not available at the default `http://192.168.0.1` address

The browser talks to Vite on the same origin, so the device does not require CORS changes

## Simulation

The scenario selector covers normal, disconnected, Goto, tracking, parked, fault, limit, mount-only, and all-subsystem states

Capability boundaries come from `fixtures/serial-replays.json`; the browser does not derive features from controller version strings
