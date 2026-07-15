import SwiftUI

struct BotsView: View {
    @ObservedObject var settings: LauncherSettings
    @State private var showAdvanced = false

    private var botCount: Binding<Int> {
        Binding(
            get: { settings.botCount },
            set: { settings.botCount = LauncherSettings.clampedBotCount($0) }
        )
    }

    // Spearhead is intentionally omitted: Breakthrough is mechanically identical
    // and a content superset (mounting mainta), so it covers everything Spearhead
    // does. The value is com_target_game, so Breakthrough stays 2 (not 1).
    private let games: [(game: Int, name: String, icon: String)] = [
        (0, "AA", "star.fill"),
        (2, "BT", "bolt.fill"),
    ]
    private let labelW: CGFloat = Theme.labelWidth
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
        ScrollView {
            VStack(spacing: Theme.sectionGap) {
                Card(spacing: 6) {
                    HStack(spacing: 8) {
                        gameSelector
                        VerifyButton()
                    }

                    FormRow("Map") {
                        Picker("", selection: $settings.botGameType) {
                            Text("FFA").tag(1)
                            Text("TDM").tag(2)
                            Text("OBJ").tag(4)
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
                        Text("Snipers")
                            .font(.system(size: 11))
                            .foregroundColor(.secondary)
                        TextField("", value: $settings.botSniper, format: .number)
                            .textFieldStyle(.roundedBorder)
                            .font(.system(size: 12))
                            .frame(width: 44)
                    }

                    Slider(value: $settings.botDifficulty, in: 0...100, step: 1)
                        .disabled(settings.botManualTuning)
                        .opacity(settings.botManualTuning ? 0.5 : 1)

                    DisclosureGroup(isExpanded: $showAdvanced) {
                        VStack(spacing: 6) {
                            Text(tuningSummary)
                                .font(.system(size: 9))
                                .foregroundColor(.secondary)
                                .frame(maxWidth: .infinity, alignment: .leading)

                            Toggle("Manual tuning (ignores the slider)", isOn: $settings.botManualTuning)
                                .toggleStyle(.checkbox)
                                .font(.system(size: 11))
                                .frame(maxWidth: .infinity, alignment: .leading)

                            advRow("React delay s", tunedBinding(\.botReactDelay, \.reactDelay), enabled: settings.botManualTuning)
                            advRow("Turn rate °/s", tunedBinding(\.botTurnSpeed, \.turnSpeed), enabled: settings.botManualTuning)
                            advRow("Turn accel", tunedBinding(\.botTurnAccel, \.turnAccel), enabled: settings.botManualTuning)
                            advRow("Aim error", tunedBinding(\.botAimError, \.aimError), enabled: settings.botManualTuning)
                            advRow("Settle time s", tunedBinding(\.botAimSettle, \.aimSettle), enabled: settings.botManualTuning)
                            advRow("Aim latency ms", tunedBinding(\.botAimLatency, \.aimLatency), enabled: settings.botManualTuning)
                            advRow("Spread", tunedBinding(\.botFirespreadScale, \.spreadScale), enabled: settings.botManualTuning)

                            HStack(spacing: 8) {
                                Text("Aim height")
                                    .font(.system(size: 11))
                                    .frame(width: labelW, alignment: .trailing)
                                    .foregroundColor(.secondary)
                                TextField("", text: $settings.botAimHeightMin)
                                    .textFieldStyle(.roundedBorder)
                                    .font(.system(size: 11))
                                TextField("", text: $settings.botAimHeightMax)
                                    .textFieldStyle(.roundedBorder)
                                    .font(.system(size: 11))
                            }
                        }
                        .padding(.top, 4)
                    } label: {
                        Text("Advanced")
                            .font(.system(size: 11))
                            .foregroundColor(.secondary)
                    }

                    FormRow("Health") {
                        TextField("", value: $settings.playerHealth, format: .number)
                            .textFieldStyle(.roundedBorder)
                            .font(.system(size: 12))
                    }

                    FormRow("Accuracy") {
                        Picker("", selection: $settings.accuracy) {
                            Text("Normal").tag(0)
                            Text("High").tag(1)
                            Text("Perfect").tag(2)
                        }
                        .pickerStyle(.segmented)
                        .labelsHidden()
                        .font(.system(size: 11))
                    }

                    FormRow("Options") {
                        Toggle("God mode", isOn: $settings.godMode)
                            .toggleStyle(.checkbox)
                            .font(.system(size: 12))
                        Toggle("AA style", isOn: $settings.aaStyle)
                            .toggleStyle(.checkbox)
                            .font(.system(size: 12))
                        Spacer()
                    }
                }

                LaunchButton(title: "Play", systemImage: "play.fill") {
                    GameLauncher.launchBots(settings: settings)
                }
            }
            .padding(Theme.pagePadding)
        }
        .onAppear { settings.load() }
        .onChange(of: settings.botCount) { _ in settings.save() }
        .onChange(of: settings.botGameType) { _ in settings.save() }
        .onChange(of: settings.botMap) { _ in settings.save() }
        .onChange(of: settings.botTeam) { _ in settings.save() }
        .onChange(of: settings.playerHealth) { _ in settings.save() }
        .onChange(of: settings.gameType) { g in
            if !maps(for: g).contains(settings.botMap) {
                settings.botMap = maps(for: g).first ?? ""
            }
            settings.save()
        }
        .onChange(of: settings.botSniper) { _ in settings.save() }
        .onChange(of: settings.botDifficulty) { _ in settings.save() }
        .onChange(of: settings.accuracy) { _ in settings.save() }
        .onChange(of: settings.aaStyle) { _ in settings.save() }
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

    private var tuningSummary: String {
        let t = settings.effectiveTuning()
        let suffix = settings.botManualTuning ? " · manual" : ""
        let spread = "\(t.spreadScale)×"
        return "react \(t.reactDelay)s · turn \(t.turnSpeed)°/s @ \(t.turnAccel) · aim \(t.aimError)/\(t.aimSettle)s · lag \(t.aimLatency)ms · spread \(spread)\(suffix)"
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

    private func advRow(_ label: String, _ text: Binding<String>, enabled: Bool) -> some View {
        HStack(spacing: 8) {
            Text(label)
                .font(.system(size: 11))
                .frame(width: labelW, alignment: .trailing)
                .foregroundColor(.secondary)
            TextField("", text: text)
                .textFieldStyle(.roundedBorder)
                .font(.system(size: 11))
                .disabled(!enabled)
                .opacity(enabled ? 1 : 0.6)
        }
    }
}
