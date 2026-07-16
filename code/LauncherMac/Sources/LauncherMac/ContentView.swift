import SwiftUI

struct ContentView: View {
    @ObservedObject var settings: LauncherSettings
    @State private var bookmarkNameInput: String = ""
    @State private var showBookmarkNaming: Int? = nil

    var body: some View {
        TabView {
            connectTab
                .tabItem { Label("Connect", systemImage: "network") }
            BotsView(settings: settings)
                .tabItem { Label("Bots", systemImage: "person.3.fill") }
            CrosshairView(settings: settings)
                .tabItem { Label("Crosshair", systemImage: "scope") }
        }
        .frame(width: 360, height: 325)
        .onChange(of: settings.ip) { _ in settings.save() }
        .onChange(of: settings.password) { _ in settings.save() }
        .onChange(of: settings.rconPassword) { _ in settings.save() }
        .onChange(of: settings.nickname) { _ in settings.save() }
        .onChange(of: settings.resolutionIndex) { _ in settings.save() }
    }

    private var connectTab: some View {
        ScrollView {
            VStack(spacing: Theme.sectionGap) {
                Card {
                    LabeledField("Nickname", text: $settings.nickname)
                    LabeledField("Server IP", text: $settings.ip)
                    LabeledField("Password", text: $settings.password, secure: true)
                    LabeledField("RCON", text: $settings.rconPassword, secure: true)
                    resolutionRow
                    ForEach(0..<maxBookmarks, id: \.self) { i in
                        bookmarkRow(index: i)
                    }
                }

                LaunchButton(title: "Connect", systemImage: "bolt.fill") {
                    GameLauncher.launch(settings: settings)
                }
            }
            .padding(Theme.pagePadding)
        }
    }

    // Resolution is always applied — pick one from the list.
    private var resolutionRow: some View {
        HStack(spacing: 8) {
            Text("Resolution")
                .font(.system(size: 12))
                .frame(width: Theme.labelWidth, alignment: .trailing)
                .foregroundColor(.secondary)
            Picker("", selection: $settings.resolutionIndex) {
                ForEach(0..<resolutionList.count, id: \.self) { i in
                    Text(resolutionList[i].label).tag(i)
                }
            }
            .labelsHidden()
            .font(.system(size: 11))
        }
    }

    @ViewBuilder
    private func bookmarkRow(index i: Int) -> some View {
        HStack(spacing: 6) {
            Button(action: { loadBookmark(i) }) {
                Text(settings.bookmarks[i].name.isEmpty ? "(empty)" : settings.bookmarks[i].name)
                    .font(.system(size: 12))
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .lineLimit(1)
                    .foregroundColor(settings.bookmarks[i].name.isEmpty ? .secondary : .primary)
            }
            .buttonStyle(.plain)
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
    }

    private func saveBookmark(_ index: Int, name: String) {
        guard !name.isEmpty else { return }
        settings.bookmarks[index].name = name
        settings.bookmarks[index].ip = settings.ip
        settings.bookmarks[index].password = settings.password
        settings.bookmarks[index].rconPassword = settings.rconPassword
        settings.save()
    }
}

struct LabeledField: View {
    let label: String
    @Binding var text: String
    var secure: Bool = false
    var labelWidth: CGFloat = Theme.labelWidth

    init(_ label: String, text: Binding<String>, secure: Bool = false, labelWidth: CGFloat = Theme.labelWidth) {
        self.label = label
        self._text = text
        self.secure = secure
        self.labelWidth = labelWidth
    }

    var body: some View {
        HStack(spacing: 8) {
            Text(label)
                .font(.system(size: 12))
                .frame(width: labelWidth, alignment: .trailing)
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
