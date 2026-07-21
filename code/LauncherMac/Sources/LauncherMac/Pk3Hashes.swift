struct Pk3Hashes {
    // Known MD5 hashes for standard retail pak files.
    // Format: "folder/filename" → MD5 hex string (lowercase).
    // Keys are normalized to lowercase for case-insensitive matching.
    static let retail: [String: String] = [
        // Allied Assault (main/)
        "main/pak0.pk3":      "26d6c5383a2318864549f6e625acde4d",
        "main/pak1.pk3":      "82cd6260d3d84beceb6b0866f329a1cf",
        "main/pak2.pk3":      "1e2e77461a03f02a9878508a7e035c2f",
        "main/pak3.pk3":      "08ae9d7ee5bb25bf9115fbe4bd071714",
        "main/pak4.pk3":      "fff8f0f1e6f9c2a6f1fb0dc194c5822b",
        "main/pak5.pk3":      "ebc75ba147d28cf5397db9cf7647c2c5",
        "main/pak6enuk.pk3":  "0dde61807fee408ca142c671e10baf33",
        "main/pak7.pk3":      "622ad6039c8369738fc54b3573f6b739",

        // Spearhead (mainta/)
        "mainta/pak1.pk3":    "f030b95c358eeed05891e0bbc9b8e361",
        "mainta/pak2.pk3":    "16fceb0c8410ae87b0c6bae1ae761c5b",
        "mainta/pak3.pk3":    "099d46871c7b7bbefbea0c72ccd6cea5",
        "mainta/pak4.pk3":    "c4cd23103785916f723658159e9962b4",
        "mainta/pak5.pk3":    "e07af969447d25d91c28f5dd00b84adc",

        // Breakthrough (maintt/)
        "maintt/pak1.pk3":    "f2ad44c4de4e7ef2b9411afe957e0527",
        "maintt/pak2.pk3":    "46b5226a8d940f5ee7101dd75b7be48a",
        "maintt/pak3.pk3":    "a3af0f4ba06a5d752105d0c237ee4db8",
        "maintt/pak4.pk3":    "9ef5bfc0588ba782e41923f81f737fb4",
    ]
}
