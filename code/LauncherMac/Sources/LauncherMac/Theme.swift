import SwiftUI

/// Shared visual tokens so every tab uses the same accent, spacing and radii.
enum Theme {
    static let accent = Color(red: 123 / 255, green: 79 / 255, blue: 191 / 255)

    static let pagePadding: CGFloat = 14   // outer margin of each tab
    static let sectionGap: CGFloat = 12    // space between cards
    static let rowGap: CGFloat = 8         // space between rows inside a card
    static let cardRadius: CGFloat = 8
    static let labelWidth: CGFloat = 88    // right-aligned label column
}

/// A grouped section: an optional small-caps header over a subtly filled,
/// rounded container. Replaces the old flat lists separated by dividers.
struct Card<Content: View, Accessory: View>: View {
    private let title: String?
    private let spacing: CGFloat
    private let accessory: Accessory
    private let content: Content

    init(_ title: String? = nil,
         spacing: CGFloat = Theme.rowGap,
         @ViewBuilder accessory: () -> Accessory = { EmptyView() },
         @ViewBuilder content: () -> Content) {
        self.title = title
        self.spacing = spacing
        self.accessory = accessory()
        self.content = content()
    }

    var body: some View {
        VStack(alignment: .leading, spacing: spacing) {
            if title != nil || Accessory.self != EmptyView.self {
                HStack(spacing: 6) {
                    if let title = title {
                        Text(title.uppercased())
                            .font(.system(size: 10, weight: .semibold))
                            .tracking(0.7)
                            .foregroundColor(.secondary)
                    }
                    Spacer(minLength: 0)
                    accessory
                }
            }
            content
        }
        .frame(maxWidth: .infinity, alignment: .leading)
        .padding(10)
        .background(
            RoundedRectangle(cornerRadius: Theme.cardRadius, style: .continuous)
                .fill(Color.primary.opacity(0.045))
        )
        .overlay(
            RoundedRectangle(cornerRadius: Theme.cardRadius, style: .continuous)
                .strokeBorder(Color.primary.opacity(0.07), lineWidth: 1)
        )
    }
}

/// A right-aligned label paired with arbitrary trailing controls, shared by
/// every settings row so the label columns line up across cards.
struct FormRow<Content: View>: View {
    private let label: String
    private let content: Content

    init(_ label: String, @ViewBuilder content: () -> Content) {
        self.label = label
        self.content = content()
    }

    var body: some View {
        HStack(spacing: 8) {
            Text(label)
                .font(.system(size: 12))
                .frame(width: Theme.labelWidth, alignment: .trailing)
                .foregroundColor(.secondary)
            content
        }
    }
}

/// Full-width accent action button (Connect / Play).
struct LaunchButton: View {
    let title: String
    let systemImage: String
    let action: () -> Void

    var body: some View {
        Button(action: action) {
            HStack(spacing: 6) {
                Image(systemName: systemImage)
                    .font(.system(size: 12, weight: .semibold))
                Text(title)
                    .font(.system(size: 14, weight: .semibold))
            }
            .foregroundColor(.white)
            .frame(maxWidth: .infinity)
            .padding(.vertical, 9)
            .contentShape(Rectangle())
        }
        .buttonStyle(.plain)
        .background(
            RoundedRectangle(cornerRadius: 8, style: .continuous)
                .fill(Theme.accent)
        )
    }
}
