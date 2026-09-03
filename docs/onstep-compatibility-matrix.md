# OnStep compatibility matrix

## Fail-closed controller detection

`Status::update` establishes the protocol family from firmware-owned serial detection:

1. `:GVP#` must return a non-empty product containing `On-Step`
2. `:GVN#` must return `<major>.<minor><lowercase-patch>`
3. Major and minor must each be within 0 through 99
4. Invalid version syntax resets the parsed version, marks the controller disconnected, clears the configuration name, and must expose read-only UI capabilities
5. A later status, meridian, or alignment failure marks the controller disconnected

The Vue application consumes normalized capabilities only. It must not parse `controller.version` or duplicate any threshold below.

## Version capability boundaries

| Behavior | OnStep 4.24 | OnStepX 10.15 | OnStepX 10.20 | OnStepX 10.26 | OnStepX 10.28+ | Firmware evidence |
| --- | --- | --- | --- | --- | --- | --- |
| Protocol family | `onstep4` | `onstepx` | `onstepx` | `onstepx` | `onstepx` | Major `<10` versus `>=10` branches |
| Axis scan ceiling | 2 | 9 | 9 | 9 | 9 | `State::updateController`, `State::axisStatusUpdate`, index page |
| Focuser discovery | Legacy `:FA#` and `:fA#`, at most 2 | `:F1a#` through `:F6a#`, at most 6 | Same | Same | Same | `Status::focuserScan` |
| Legacy workload | `:GXFA#` | Not queried | Not queried | Not queried | Not queried | `State::updateController` |
| High-precision site coordinates | `:GtH#` and `:GgH#` | Same | Same | Same | Same | Major `>3` branches in `State::updateMount` and `siteTileGet` |
| High-precision Az/Alt | Not used | Available when compiled on | Same | Same | Same | Major `>=10` branch in `State::updateMount` |
| Working focuser/rotator slew rate | Not queried | Available | Available | Available | Available | Major `>=10` branches in focuser and rotator state |
| Encoder sync during goto | Legacy restrictions | Available from 10.15 | Available | Available | Available | Threshold `>=1015` in `Encoders::poll` |
| Auto-home controls | Hidden | Hidden | Available | Available | Available | Threshold `>=1020` in `HomeParkTile` |
| Hardware name | Hidden | Hidden | Hidden | Available | Available | Threshold 10.26 in `StatusTile` |
| Axis configuration UI/actions | Hidden | Hidden | Hidden | Available when `DRIVE_CONFIGURATION == ON` | Available when compiled on | Threshold 10.26 in `AxisTile` |
| Meridian-flip home modes | Hidden | Hidden | Hidden | Hidden | Available for GEM/Fork | `Status::hasMeridianFlipHomeModes`, threshold 10.28 |

## Compile-time and runtime capability composition

The firmware must build `/ui-config.json` and `/api/v1/capabilities` from both compile-time flags and runtime discovery.

| Capability | Required sources | Safe fallback |
| --- | --- | --- |
| `mount` | `status.onStepFound` and `status.mountFound == SD_TRUE` | `false` |
| `rotator` | Runtime `status.rotatorFound == SD_TRUE` | `false` |
| `focuserCount` | Runtime `status.focuserCount`, clamped to protocol maximum | `0` |
| `auxiliary` | Runtime `status.auxiliaryFound == SD_TRUE` | `false` |
| `encoders` | Compile-time `ENCODERS == ON` and valid runtime support | `false` |
| `axisCount` | Firmware protocol branch plus discovered valid axes | `2` for valid OnStep 4.x, otherwise `0` when unknown |
| `highPrecisionCoordinates` | Relevant display/build flag and firmware command support | `false` when support is unknown |
| `axisConfiguration` | OnStepX 10.26+ and `DRIVE_CONFIGURATION == ON` | `false` |
| `autoHome` | OnStepX 10.20+ plus home support where the action requires it | `false` |
| `meridianFlipHomeModes` | OnStepX 10.28+ and GEM/Fork runtime mount type | `false` |
| `workingSlewRate` | OnStepX major 10+ and discovered focuser/rotator context | `false` |

An unknown version, malformed reply, or disconnected controller must set every action capability to false. Read-only diagnostic identity may remain visible, but stale state must never enable actions.

## Existing state ownership

- `Status::update` owns connection validity, mount state flags, park/home/goto/tracking state, PEC state, guide state, mount type, meridian behavior, alignment, and error state
- `State::updateController` owns weather, driver status, controller temperature, error text, legacy workload, Wi-Fi signal, and axis status
- `State::updateMount` owns coordinates, site, pier side, polar alignment, park/tracking presentation, and slew rate
- `State::updateFocuser` preserves separate OnStep 4.x and OnStepX focuser selection behavior
- `State::updateRotator` only queries working slew rate for OnStepX
- `State::updateAuxiliary` parses eight features and the optional 10.27f power-monitor suffix
- `State::updateEncoders` is compiled behind `ENCODERS == ON`

Polling references are centralized in `State::poll`, `State::pollSlow`, and `State::pollFast`. New APIs should serialize these typed firmware states rather than issue an independent set of serial commands.

## Existing action ownership

Current GET handlers translate UI keys to OnStep commands in firmware:

- Controller: advanced mode, reset, NV wipe, firmware-update reset, and mount type
- Mount: site/time, Home/Park, alignment, Goto, guide motion, tracking, PEC, coordinate origin, limits, and catalog operations
- Focuser: selection, home, motion/rate, position, backlash, deadband, and temperature compensation
- Rotator: home, motion/rate, position, derotation, and backlash
- Auxiliary: feature values and mode-specific conversions
- Encoders: sync/origin, auto-sync, thresholds, counts per degree, and direction
- Network: login/logout, timeout values, AP/station/Ethernet configuration, OTA, persistence, and restart semantics

Version-specific serial commands remain in these firmware handlers until semantic action functions are extracted. The Vue application must call one action adapter and must never contain strings such as `:Mn#`, `:FA1#`, `:SX93,3#`, or any other OnStep command.

## Migration invariants

- Preserve the `<10` legacy branches and the `>3` high-precision site command branches for OnStep 4.24
- Preserve the 10.15, 10.20, 10.26, 10.27f, and 10.28 thresholds
- Keep old GET routes and text-state formats unchanged through at least one stable release after the versioned API ships
- New POST handlers use `application/x-www-form-urlencoded` and the existing `www.arg()` abstraction
- Old GET handlers and new POST handlers must call the same semantic firmware actions before adapters migrate
- Motion actions must fail closed when state is stale or disconnected and must always have an idempotent stop path
