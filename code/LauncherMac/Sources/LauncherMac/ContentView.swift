import SwiftUI

struct ContentView: View {
    @ObservedObject var settings: LauncherSettings
    @State private var bookmarkNameInput: String = ""
    @State private var showBookmarkNaming: Int? = nil

    private let gameIcons = ["star.fill", "shield.fill", "bolt.fill"]
    private let gameNames = ["Allied Assault", "Spearhead", "Breakthrough"]
    private let purple = Color(red: 123/255, green: 79/255, blue: 191/255)

    var body: some View {
        VStack(spacing: 8) {
            LabeledField("Nickname", text: $settings.nickname)
            LabeledField("Server IP", text: $settings.ip)
            LabeledField("Password", text: $settings.password, secure: true)
            LabeledField("RCON", text: $settings.rconPassword, secure: true)

            // Resolution — checkbox on the left, picker appears to the right
            HStack {
                Toggle("", isOn: $settings.overrideResolution)
                    .toggleStyle(.checkbox)
                    .labelsHidden()
                if settings.overrideResolution {
                    Picker("", selection: $settings.resolutionIndex) {
                        ForEach(0..<resolutionList.count, id: \.self) { i in
                            Text(resolutionList[i].label).tag(i)
                        }
                        Text("Custom").tag(resolutionList.count)
                    }
                    .labelsHidden()
                    .font(.system(size: 11))
                } else {
                    Text("Resolution")
                        .font(.system(size: 12))
                        .foregroundColor(.secondary)
                }
            }

            if settings.overrideResolution && settings.resolutionIndex == resolutionList.count {
                HStack {
                    TextField("Width", value: $settings.customWidth, format: .number)
                        .frame(width: 70)
                        .textFieldStyle(.roundedBorder)
                    Text("x")
                    TextField("Height", value: $settings.customHeight, format: .number)
                        .frame(width: 70)
                        .textFieldStyle(.roundedBorder)
                }
            }

            Divider()

            // Game version selector — icon + name buttons
            HStack(spacing: 0) {
                ForEach(0..<3, id: \.self) { i in
                    Button(action: { settings.gameType = i }) {
                        HStack(spacing: 4) {
                            Image(systemName: gameIcons[i])
                                .font(.system(size: 11))
                            Text(gameNames[i])
                                .font(.system(size: 11))
                        }
                        .frame(maxWidth: .infinity)
                        .padding(.vertical, 5)
                        .foregroundColor(settings.gameType == i ? .white : .secondary)
                        .background(settings.gameType == i ? purple.opacity(0.8) : Color.clear)
                    }
                    .buttonStyle(.plain)
                }
            }
            .background(Color.gray.opacity(0.2))
            .cornerRadius(5)

            Divider()

            // Bookmarks
            VStack(alignment: .leading, spacing: 4) {
                Text("Bookmarks").font(.subheadline).foregroundColor(.secondary)
                ForEach(0..<maxBookmarks, id: \.self) { i in
                    bookmarkRow(index: i)
                }
            }

            // Connect button
            Button(action: {
                GameLauncher.launch(settings: settings)
            }) {
                Text("Connect")
                    .font(.headline)
                    .foregroundColor(.white)
                    .frame(maxWidth: .infinity)
                    .padding(.vertical, 6)
            }
            .buttonStyle(.plain)
            .background(purple)
            .cornerRadius(6)
        }
        .padding(12)
        .frame(width: 340)
        .fixedSize(horizontal: false, vertical: true)
        .onAppear {
            settings.load()
        }
        .onChange(of: settings.ip) { _ in settings.save() }
        .onChange(of: settings.password) { _ in settings.save() }
        .onChange(of: settings.rconPassword) { _ in settings.save() }
        .onChange(of: settings.nickname) { _ in settings.save() }
        .onChange(of: settings.gameType) { _ in settings.save() }
        .onChange(of: settings.overrideResolution) { _ in settings.save() }
        .onChange(of: settings.resolutionIndex) { _ in settings.save() }
    }

    @ViewBuilder
    private func bookmarkRow(index i: Int) -> some View {
        HStack(spacing: 4) {
            Button(action: { loadBookmark(i) }) {
                Text(settings.bookmarks[i].name.isEmpty ? "(empty)" : settings.bookmarks[i].name)
                    .font(.system(size: 12))
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .lineLimit(1)
            }
            .disabled(settings.bookmarks[i].name.isEmpty)

            Button(action: {
                loadBookmark(i)
                GameLauncher.launch(settings: settings)
            }) {
                Image(systemName: "play.fill")
                    .font(.system(size: 10))
            }
            .disabled(settings.bookmarks[i].name.isEmpty)

            Button("Save") {
                let defaultName = !settings.bookmarks[i].name.isEmpty
                    ? settings.bookmarks[i].name
                    : (!settings.ip.isEmpty ? settings.ip : "Bookmark \(i + 1)")
                bookmarkNameInput = defaultName
                showBookmarkNaming = i
            }
            .font(.system(size: 11))

            Button(action: {
                settings.bookmarks[i] = Bookmark()
                settings.save()
            }) {
                Image(systemName: "trash").foregroundColor(.red).font(.system(size: 10))
            }
            .frame(width: 20)
            .disabled(settings.bookmarks[i].name.isEmpty)
        }
        .sheet(isPresented: Binding(
            get: { showBookmarkNaming == i },
            set: { if !$0 { showBookmarkNaming = nil } }
        )) {
            VStack(spacing: 12) {
                Text("Bookmark Name").font(.headline)
                TextField("Name", text: $bookmarkNameInput)
                    .textFieldStyle(.roundedBorder)
                    .frame(width: 200)
                HStack {
                    Button("Cancel") { showBookmarkNaming = nil }
                    Button("OK") {
                        saveBookmark(i, name: bookmarkNameInput)
                        showBookmarkNaming = nil
                    }
                    .keyboardShortcut(.defaultAction)
                }
            }
            .padding()
            .frame(width: 260)
        }
    }

    private func loadBookmark(_ index: Int) {
        let bm = settings.bookmarks[index]
        settings.ip = bm.ip
        settings.password = bm.password
        settings.rconPassword = bm.rconPassword
        settings.gameType = bm.gameType
    }

    private func saveBookmark(_ index: Int, name: String) {
        guard !name.isEmpty else { return }
        settings.bookmarks[index].name = name
        settings.bookmarks[index].ip = settings.ip
        settings.bookmarks[index].password = settings.password
        settings.bookmarks[index].rconPassword = settings.rconPassword
        settings.bookmarks[index].gameType = settings.gameType
        settings.save()
    }
}

struct LabeledField: View {
    let label: String
    @Binding var text: String
    var secure: Bool = false

    init(_ label: String, text: Binding<String>, secure: Bool = false) {
        self.label = label
        self._text = text
        self.secure = secure
    }

    var body: some View {
        HStack {
            Text(label)
                .font(.system(size: 12))
                .frame(width: 70, alignment: .trailing)
                .foregroundColor(.secondary)
            if secure {
                SecureField("", text: $text)
                    .textFieldStyle(.roundedBorder)
                    .font(.system(size: 12))
            } else {
                TextField("", text: $text)
                    .textFieldStyle(.roundedBorder)
                    .font(.system(size: 12))
            }
        }
    }
}
