import AppKit
import SwiftUI

struct CrosshairView: View {
    @ObservedObject var settings: LauncherSettings

    private var resolution: ResolutionEntry {
        guard resolutionList.indices.contains(settings.resolutionIndex) else {
            return resolutionList[resolutionList.count - 1]
        }
        return resolutionList[settings.resolutionIndex]
    }

    private var previewColor: Color {
        let rgb = Int(LauncherSettings.validatedCrosshairColor(settings.crosshairColor), radix: 16) ?? 0xFFFFFF
        return Color(
            red: Double((rgb >> 16) & 0xFF) / 255,
            green: Double((rgb >> 8) & 0xFF) / 255,
            blue: Double(rgb & 0xFF) / 255
        )
    }

    private var colorBinding: Binding<Color> {
        Binding(
            get: { previewColor },
            set: { color in
                guard let srgb = NSColor(color).usingColorSpace(.sRGB) else { return }
                let red = min(max(Int((srgb.redComponent * 255).rounded()), 0), 255)
                let green = min(max(Int((srgb.greenComponent * 255).rounded()), 0), 255)
                let blue = min(max(Int((srgb.blueComponent * 255).rounded()), 0), 255)
                settings.crosshairColor = String(format: "%02X%02X%02X", red, green, blue)
            }
        )
    }

    var body: some View {
        ScrollView {
            VStack(spacing: Theme.sectionGap) {
                Card("Preview") {
                    CrosshairPreview(
                        color: previewColor,
                        length: settings.crosshairLength,
                        gap: settings.crosshairGap,
                        thickness: settings.crosshairThickness,
                        resolutionHeight: resolution.height ?? 1080
                    )
                    .frame(maxWidth: .infinity)
                    .frame(height: 112)
                    .background(
                        RoundedRectangle(cornerRadius: 6, style: .continuous)
                            .fill(Color.black.opacity(0.82))
                    )

                    Text("\(resolution.label) · dimensions scale from 1080p")
                        .font(.system(size: 10))
                        .foregroundColor(.secondary)
                        .frame(maxWidth: .infinity, alignment: .center)
                }

                Card("Crosshair") {
                    Toggle("Enable custom crosshair", isOn: $settings.crosshairEnabled)
                        .toggleStyle(.checkbox)
                        .font(.system(size: 12))

                    FormRow("Color") {
                        ColorPicker("", selection: colorBinding, supportsOpacity: false)
                            .labelsHidden()
                        Text("#\(LauncherSettings.validatedCrosshairColor(settings.crosshairColor))")
                            .font(.system(size: 11, design: .monospaced))
                            .foregroundColor(.secondary)
                        Spacer()
                    }

                    sliderRow("Arm length", value: $settings.crosshairLength, range: 2...32)
                    sliderRow("Center gap", value: $settings.crosshairGap, range: 1...20)
                    sliderRow("Thickness", value: $settings.crosshairThickness, range: 1...8)

                    HStack {
                        Spacer()
                        Button("Reset") { settings.resetCrosshair() }
                            .font(.system(size: 11))
                    }
                }
            }
            .padding(Theme.pagePadding)
        }
        .onChange(of: settings.crosshairEnabled) { _ in settings.save() }
        .onChange(of: settings.crosshairLength) { _ in settings.save() }
        .onChange(of: settings.crosshairGap) { _ in settings.save() }
        .onChange(of: settings.crosshairThickness) { _ in settings.save() }
        .onChange(of: settings.crosshairColor) { _ in settings.save() }
    }

    private func sliderRow(_ label: String, value: Binding<Double>, range: ClosedRange<Double>) -> some View {
        FormRow(label) {
            Slider(value: value, in: range, step: 1)
            Text("\(Int(value.wrappedValue))")
                .font(.system(size: 11, design: .monospaced))
                .frame(width: 24, alignment: .trailing)
        }
    }
}

private struct CrosshairPreview: View {
    let color: Color
    let length: Double
    let gap: Double
    let thickness: Double
    let resolutionHeight: Int

    var body: some View {
        Canvas { context, size in
            let scale = Double(resolutionHeight) / 1080
            let scaledLength = max(1, (length * scale).rounded())
            let scaledThickness = max(1, (thickness * scale).rounded())
            let scaledGap = max(
                max(1, (gap * scale).rounded()),
                scaledThickness / 2 + 0.5
            )
            let centerX = size.width / 2
            let centerY = size.height / 2
            let halfThickness = scaledThickness / 2

            let arms = [
                CGRect(
                    x: centerX - scaledGap - scaledLength,
                    y: centerY - halfThickness,
                    width: scaledLength,
                    height: scaledThickness
                ),
                CGRect(
                    x: centerX + scaledGap,
                    y: centerY - halfThickness,
                    width: scaledLength,
                    height: scaledThickness
                ),
                CGRect(
                    x: centerX - halfThickness,
                    y: centerY - scaledGap - scaledLength,
                    width: scaledThickness,
                    height: scaledLength
                ),
                CGRect(
                    x: centerX - halfThickness,
                    y: centerY + scaledGap,
                    width: scaledThickness,
                    height: scaledLength
                ),
            ]

            for arm in arms {
                context.fill(Path(arm), with: .color(color))
            }
        }
        .accessibilityLabel("Custom crosshair preview")
    }
}
