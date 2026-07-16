import Foundation
import SwiftUI

struct BotsView: View {
    @ObservedObject var settings: LauncherSettings

    private var botCount: Binding<Int> {
        Binding(
            get: { settings.botCount },
            set: { settings.botCount = LauncherSettings.clampedBotCount($0) }
        )
    }

    private var runSpeed: Binding<Double> {
        Binding(
            get: { settings.runSpeed },
            set: { settings.runSpeed = LauncherSettings.clampedRunSpeed($0) }
        )
    }

    private var botStg: Binding<Double> {
        Binding(
            get: { LauncherSettings.clampedPercentage(settings.botStg) },
            set: { settings.botStg = LauncherSettings.clampedPercentage($0) }
        )
    }

    private var botSniper: Binding<Double> {
        Binding(
            get: { Double(LauncherSettings.clampedPercentage(settings.botSniper)) },
            set: { settings.botSniper = LauncherSettings.clampedPercentage(Int($0.rounded())) }
        )
    }

    private var playerHealth: Binding<Int> {
        Binding(
            get: { LauncherSettings.clampedPlayerHealth(settings.playerHealth) },
            set: { settings.playerHealth = LauncherSettings.clampedPlayerHealth($0) }
        )
    }

    // Spearhead is intentionally omitted: Breakthrough is mechanically identical
    // and a content superset (mounting mainta), so it covers everything Spearhead
    // does. The value is com_target_game, so Breakthrough stays 2 (not 1).
    private let games: [(game: Int, name: String, icon: String)] = [
        (0, "AA", "star.fill"),
        (2, "BT", "bolt.fill"),
    ]
    private let alliedMaps = [
        "dm/downladder", "dm/brownffa", "dm/vents", "dm/flag", "dm/main",
        "dm/alpha", "dm/crnodoors", "dm/mohdm1", "dm/mohdm6", "dm/mohdm7",
        "obj/obj_team2", "obj/obj_team4",
    ]
    // Spearhead stock MP maps: deathmatch (dm/) + tug-of-war (obj/).
    private let spearheadMaps = [
        "dm/MP_Bahnhof_DM", "dm/MP_Bazaar_DM", "dm/MP_Brest_DM",
        "dm/MP_Gewitter_DM", "dm/MP_Holland_DM", "dm/MP_Malta_DM",
        "dm/MP_Stadt_DM", "dm/MP_Unterseite_DM", "dm/MP_Verschneit_DM",
        "obj/MP_Ardennes_TOW", "obj/MP_Berlin_TOW", "obj/MP_Druckkammern_TOW",
        "obj/MP_Flughafen_TOW",
    ]
    // Breakthrough stock MP maps: objective (obj/), tug-of-war (obj/),
    // liberation (lib/). No pure deathmatch maps ship with Breakthrough;
    // these run in TDM/FFA thanks to the team-map gametype allowance.
    private let breakthroughMaps = [
        "obj/MP_BizerteFort_OBJ", "obj/MP_Bologna_OBJ", "obj/MP_Castello_OBJ",
        "obj/MP_Palermo_OBJ", "obj/MP_Kasserine_TOW", "obj/MP_MonteBattaglia_TOW",
        "obj/MP_MonteCassino_TOW", "lib/MP_Anzio_LIB", "lib/MP_BizerteHarbor_LIB",
        "lib/MP_Ship_LIB", "lib/MP_Tunisia_LIB",
    ]

    // Spearhead ships the Allied Assault maps too, and Breakthrough ships
    // both — so each game's list is cumulative, its own maps first.
    private func maps(for game: Int) -> [String] {
        switch game {
        case 2:  return breakthroughMaps + spearheadMaps + alliedMaps
        default: return alliedMaps
        }
    }

    var body: some View {
        VStack(spacing: 8) {
            Card("Bot Match", spacing: 5) {
                    HStack(spacing: 8) {
                        gameSelector
                            .frame(width: 116)

                        Spacer(minLength: 0)

                        Text("Resolution")
                            .font(.system(size: 10))
                            .foregroundColor(.secondary)

                        Picker("", selection: $settings.resolutionIndex) {
                            ForEach(0..<resolutionList.count, id: \.self) { i in
                                Text(resolutionList[i].label).tag(i)
                            }
                        }
                        .labelsHidden()
                        .font(.system(size: 10))
                        .frame(width: 132)
                    }
                    .frame(height: 34)

                    FormRow("Map") {
                        Picker("", selection: $settings.botGameType) {
                            Text("FFA").tag(1)
                            Text("TDM").tag(2)
                        }
                        .labelsHidden()
                        .font(.system(size: 12))
                        .frame(width: 70)
                        Picker("", selection: $settings.botMap) {
                            ForEach(maps(for: settings.gameType), id: \.self) { m in
                                Text(m).tag(m)
                            }
                        }
                        .labelsHidden()
                        .font(.system(size: 12))
                    }

                    FormRow("Bots / Team") {
                        TextField("", value: botCount, format: .number)
                            .textFieldStyle(.roundedBorder)
                            .font(.system(size: 12))
                            .frame(width: 44)
                        Picker("", selection: $settings.botTeam) {
                            Text("Auto").tag("auto")
                            Text("Allies").tag("allies")
                            Text("Axis").tag("axis")
                        }
                        .labelsHidden()
                        .font(.system(size: 12))
                        .frame(width: 110)

                        Spacer(minLength: 0)

                        Text("Health")
                            .font(.system(size: 10))
                            .foregroundColor(.secondary)
                        TextField("", value: playerHealth, format: .number)
                            .textFieldStyle(.roundedBorder)
                            .font(.system(size: 12))
                            .frame(width: 58)
                    }

                    HStack(spacing: 14) {
                        percentageSlider(
                            "Sniper %", value: botSniper,
                            help: "Percentage of bot spawns that receive a sniper rifle."
                        )
                        percentageSlider(
                            "STG %", value: botStg,
                            help: "Percentage of Axis bot spawns that receive an STG. Snipers take priority if the percentages exceed 100%."
                        )
                    }

                    FormRow("Run speed") {
                        // No step parameter: macOS otherwise draws a notch for
                        // every discrete speed value.
                        Slider(
                            value: runSpeed,
                            in: LauncherSettings.minRunSpeed...LauncherSettings.maxRunSpeed
                        )
                        .controlSize(.small)
                        Text("\(Int(settings.runSpeed))")
                            .font(.system(size: 10, design: .monospaced))
                            .monospacedDigit()
                            .frame(width: 32, alignment: .trailing)
                    }
                    .help("AA default: 250 · SH/BT default: 287")

                    FormRow("Player accuracy") {
                        Picker("", selection: $settings.accuracy) {
                            Text("Normal").tag(0)
                            Text("High").tag(1)
                            Text("Perfect").tag(2)
                        }
                        .pickerStyle(.segmented)
                        .labelsHidden()
                        .font(.system(size: 11))
                    }

                    FormRow("Style") {
                        Toggle("AA lean", isOn: $settings.aaLean)
                            .toggleStyle(.checkbox)
                            .font(.system(size: 12))
                            .help("Use Allied Assault lean limits, speeds, recovery, and camera roll in Breakthrough.")
                        Toggle("SH/BT pain", isOn: $settings.painAnimations)
                            .toggleStyle(.checkbox)
                            .font(.system(size: 12))
                            .help("Play Spearhead/Breakthrough hit-reaction animations when players are shot.")
                        Spacer()
                    }

                    FormRow("Options") {
                        Toggle("God mode", isOn: $settings.godMode)
                            .toggleStyle(.checkbox)
                            .font(.system(size: 12))
                        Spacer()
                    }

                    Divider()
                        .padding(.vertical, 2)

                    FormRow("Difficulty") {
                        Slider(value: $settings.botDifficulty, in: 0...100)
                            .controlSize(.small)
                            .disabled(settings.botManualTuning)
                        Text("\(Int(settings.botDifficulty.rounded()))")
                            .font(.system(size: 10, design: .monospaced))
                            .monospacedDigit()
                            .frame(width: 24, alignment: .trailing)
                    }
                    .opacity(settings.botManualTuning ? 0.5 : 1)

                    advancedTuningControls
            }

            LaunchButton(title: "Play", systemImage: "play.fill") {
                GameLauncher.launchBots(settings: settings)
            }
        }
        .onChange(of: settings.botCount) { _ in settings.save() }
        .onChange(of: settings.botGameType) { _ in settings.save() }
        .onChange(of: settings.botMap) { _ in settings.save() }
        .onChange(of: settings.botTeam) { _ in settings.save() }
        .onChange(of: settings.playerHealth) { _ in settings.save() }
        .onChange(of: settings.runSpeed) { _ in settings.save() }
        .onChange(of: settings.gameType) { g in
            if g == 2 && settings.runSpeed == LauncherSettings.aaDefaultRunSpeed {
                settings.runSpeed = LauncherSettings.expansionDefaultRunSpeed
            } else if g == 0 && settings.runSpeed == LauncherSettings.expansionDefaultRunSpeed {
                settings.runSpeed = LauncherSettings.aaDefaultRunSpeed
            }
            if !maps(for: g).contains(settings.botMap) {
                settings.botMap = maps(for: g).first ?? ""
            }
            settings.save()
        }
        .onChange(of: settings.botSniper) { _ in settings.save() }
        .onChange(of: settings.botStg) { _ in settings.save() }
        .onChange(of: settings.botDifficulty) { _ in settings.save() }
        .onChange(of: settings.accuracy) { _ in settings.save() }
        .onChange(of: settings.aaLean) { _ in settings.save() }
        .onChange(of: settings.painAnimations) { _ in settings.save() }
        .onChange(of: settings.godMode) { _ in settings.save() }
        .onChange(of: settings.botManualTuning) { on in
            if on {
                let t = settings.derivedTuning()
                settings.botReactDelay = t.reactDelay
                settings.botTurnSpeed = t.turnSpeed
                settings.botTurnAccel = t.turnAccel
                settings.botAimError = t.aimError
                settings.botAimSettle = t.aimSettle
                settings.botAimLatency = t.aimLatency
                settings.botFirespreadScale = t.spreadScale
            }
            settings.save()
        }
        // Manual-tuning fields persist on edit like every other Bots field.
        .onChange(of: settings.botReactDelay) { _ in settings.save() }
        .onChange(of: settings.botTurnSpeed) { _ in settings.save() }
        .onChange(of: settings.botTurnAccel) { _ in settings.save() }
        .onChange(of: settings.botAimError) { _ in settings.save() }
        .onChange(of: settings.botAimSettle) { _ in settings.save() }
        .onChange(of: settings.botAimLatency) { _ in settings.save() }
        .onChange(of: settings.botFirespreadScale) { _ in settings.save() }
        .onChange(of: settings.botAimHeightMin) { _ in settings.save() }
        .onChange(of: settings.botAimHeightMax) { _ in settings.save() }
    }

    private var gameSelector: some View {
        HStack(spacing: 0) {
            ForEach(games, id: \.game) { g in
                Button(action: { settings.gameType = g.game }) {
                    HStack(spacing: 4) {
                        Image(systemName: g.icon)
                            .font(.system(size: 11))
                        Text(g.name)
                            .font(.system(size: 11, weight: settings.gameType == g.game ? .semibold : .regular))
                    }
                    .frame(maxWidth: .infinity)
                    .padding(.vertical, 6)
                    .contentShape(Rectangle())
                    .foregroundColor(settings.gameType == g.game ? .white : .secondary)
                    .background(settings.gameType == g.game ? Theme.accent : Color.clear)
                }
                .buttonStyle(.plain)
            }
        }
        .background(Color.primary.opacity(0.06))
        .clipShape(RoundedRectangle(cornerRadius: 7, style: .continuous))
    }

    private func percentageSlider(
        _ label: String,
        value: Binding<Double>,
        help: String
    ) -> some View {
        VStack(spacing: 2) {
            ZStack {
                Text(label)
                    .font(.system(size: 10))
                    .foregroundColor(.secondary)
                    .frame(maxWidth: .infinity, alignment: .center)

                HStack {
                    Spacer()
                    Text("\(Int(value.wrappedValue.rounded()))")
                        .font(.system(size: 9, design: .monospaced))
                        .monospacedDigit()
                }
            }

            Slider(value: value, in: 0...100)
                .controlSize(.small)
                .accessibilityLabel(Text(label))
        }
        .frame(maxWidth: .infinity)
        .help(help)
    }

    private var tuningSummary: String {
        let t = settings.effectiveTuning()
        let suffix = settings.botManualTuning ? " · manual" : ""
        let spread = "\(t.spreadScale)×"
        return "react \(t.reactDelay)s · turn \(t.turnSpeed)°/s @ \(t.turnAccel) · aim \(t.aimError)/\(t.aimSettle)s · lag \(t.aimLatency)ms · spread \(spread)\(suffix)"
    }

    private var advancedTuningControls: some View {
        VStack(alignment: .leading, spacing: 8) {
            HStack(spacing: 8) {
                Text("ADVANCED TUNING")
                    .font(.system(size: 10, weight: .semibold))
                    .tracking(0.7)
                    .foregroundColor(.secondary)

                Spacer()

                Toggle("Manual difficulty", isOn: $settings.botManualTuning)
                    .toggleStyle(.checkbox)
                    .font(.system(size: 11))
                    .help("Override the difficulty slider values. Aim height remains independently adjustable.")
            }

            Text(tuningSummary)
                .font(.system(size: 9))
                .foregroundColor(.secondary)
                .lineLimit(1)

            HStack(alignment: .top, spacing: 14) {
                VStack(spacing: 8) {
                    compactSlider(
                        "React delay", tunedBinding(\.botReactDelay, \.reactDelay),
                        in: 0...10, fractionDigits: 2, suffix: "s",
                        enabled: settings.botManualTuning
                    )
                    compactSlider(
                        "Turn rate", tunedBinding(\.botTurnSpeed, \.turnSpeed),
                        in: 1...1080, fractionDigits: 0, suffix: "°/s",
                        enabled: settings.botManualTuning
                    )
                    compactSlider(
                        "Turn accel", tunedBinding(\.botTurnAccel, \.turnAccel),
                        in: 0.1...100, fractionDigits: 1,
                        enabled: settings.botManualTuning
                    )
                }
                .frame(maxWidth: .infinity)

                VStack(spacing: 8) {
                    compactSlider(
                        "Aim error", tunedBinding(\.botAimError, \.aimError),
                        in: 0...400, fractionDigits: 0,
                        enabled: settings.botManualTuning
                    )
                    compactSlider(
                        "Settle time", tunedBinding(\.botAimSettle, \.aimSettle),
                        in: 0...10, fractionDigits: 2, suffix: "s",
                        enabled: settings.botManualTuning
                    )
                    compactSlider(
                        "Aim latency", tunedBinding(\.botAimLatency, \.aimLatency),
                        in: 0...2000, fractionDigits: 0, suffix: "ms",
                        enabled: settings.botManualTuning
                    )
                }
                .frame(maxWidth: .infinity)

                VStack(spacing: 8) {
                    compactSlider(
                        "Spread", tunedBinding(\.botFirespreadScale, \.spreadScale),
                        in: 0...10, fractionDigits: 2,
                        enabled: settings.botManualTuning
                    )
                    compactSlider(
                        "Aim height min", $settings.botAimHeightMin,
                        in: 0...1, fractionDigits: 2, enabled: true,
                        help: "Independent of Manual difficulty. Sets the lowest vertical aim point."
                    )
                    compactSlider(
                        "Aim height max", $settings.botAimHeightMax,
                        in: 0...1, fractionDigits: 2, enabled: true,
                        help: "Independent of Manual difficulty. Sets the highest vertical aim point."
                    )
                }
                .frame(maxWidth: .infinity)
            }
        }
        .padding(.top, 2)
    }

    private func tunedBinding(
        _ storage: ReferenceWritableKeyPath<LauncherSettings, String>,
        _ derived: KeyPath<BotTuning, String>
    ) -> Binding<String> {
        Binding(
            get: {
                settings.botManualTuning
                    ? settings[keyPath: storage]
                    : settings.derivedTuning()[keyPath: derived]
            },
            set: { settings[keyPath: storage] = $0 }
        )
    }

    private func numericBinding(
        _ text: Binding<String>,
        in range: ClosedRange<Double>,
        fractionDigits: Int
    ) -> Binding<Double> {
        Binding(
            get: {
                guard let value = Double(text.wrappedValue), value.isFinite else {
                    return range.lowerBound
                }
                return min(max(value, range.lowerBound), range.upperBound)
            },
            set: { newValue in
                let clamped = min(max(newValue, range.lowerBound), range.upperBound)
                text.wrappedValue = String(format: "%.\(fractionDigits)f", clamped)
            }
        )
    }

    private func sliderValue(_ value: Double, fractionDigits: Int) -> String {
        String(format: "%.\(fractionDigits)f", value)
    }

    private func compactSlider(
        _ label: String,
        _ text: Binding<String>,
        in range: ClosedRange<Double>,
        fractionDigits: Int,
        suffix: String = "",
        enabled: Bool,
        help: String? = nil
    ) -> some View {
        let value = numericBinding(text, in: range, fractionDigits: fractionDigits)

        return VStack(spacing: 2) {
            HStack(spacing: 4) {
                Text(label)
                    .font(.system(size: 10))
                    .foregroundColor(.secondary)
                Spacer(minLength: 4)
                Text("\(sliderValue(value.wrappedValue, fractionDigits: fractionDigits))\(suffix)")
                    .font(.system(size: 9, design: .monospaced))
                    .monospacedDigit()
            }

            Slider(value: value, in: range)
                .controlSize(.small)
                .disabled(!enabled)
                .accessibilityLabel(Text(label))
        }
        .opacity(enabled ? 1 : 0.6)
        .help(help ?? "Accepted range: \(sliderValue(range.lowerBound, fractionDigits: fractionDigits))–\(sliderValue(range.upperBound, fractionDigits: fractionDigits))")
    }
}
