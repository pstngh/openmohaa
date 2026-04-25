#!/usr/bin/env python3
import os
import sys
import subprocess
import tkinter as tk
from tkinter import ttk, messagebox

SCRIPT_DIR = os.path.dirname(os.path.abspath(sys.argv[0]))
CFG_PATH = os.path.join(SCRIPT_DIR, "launcher.cfg")
MAX_BOOKMARKS = 4

RESOLUTIONS = [
    ("800x600 (4:3) FOV 80.00", 4),
    ("960x720 (4:3) FOV 80.00", 5),
    ("1024x768 (4:3) FOV 80.00", 6),
    ("1152x864 (4:3) FOV 80.00", 7),
    ("1280x720 (16:9) FOV 96.42", 0),
    ("1280x1024 (5:4) FOV 80.00", 8),
    ("1600x1200 (4:3) FOV 80.00", 9),
    ("1920x1080 (16:9) FOV 96.42", 1),
    ("2560x1440 (16:9) FOV 96.42", 10),
    ("3840x2160 (16:9) FOV 96.42", 11),
]

GAMES = [
    ("★ Allied Assault", 0),
    ("● Spearhead", 1),
    ("⚡ Breakthrough", 2),
]


class Settings:
    def __init__(self):
        self.ip = ""
        self.password = ""
        self.rcon = ""
        self.nickname = ""
        self.game = 0
        self.override_resolution = False
        self.resolution_index = 7
        self.custom_width = 1920
        self.custom_height = 1080
        self.bookmarks = [{"name": "", "ip": "", "password": "", "rcon": "", "game": 0} for _ in range(MAX_BOOKMARKS)]

    def load(self):
        if not os.path.isfile(CFG_PATH):
            return
        with open(CFG_PATH, "r") as f:
            for line in f:
                line = line.strip()
                if "=" not in line:
                    continue
                key, value = line.split("=", 1)
                if key == "ip": self.ip = value
                elif key == "password": self.password = value
                elif key == "rcon": self.rcon = value
                elif key == "nickname": self.nickname = value
                elif key == "game": self.game = int(value) if value.lstrip("-").isdigit() else 0
                elif key == "override_resolution": self.override_resolution = value != "0"
                elif key == "resolution_index": self.resolution_index = int(value) if value.isdigit() else 7
                elif key == "custom_width": self.custom_width = int(value) if value.isdigit() else 1920
                elif key == "custom_height": self.custom_height = int(value) if value.isdigit() else 1080
                else:
                    for i in range(MAX_BOOKMARKS):
                        s = f"_{i}"
                        if key == "bookmark_name" + s: self.bookmarks[i]["name"] = value
                        elif key == "bookmark_ip" + s: self.bookmarks[i]["ip"] = value
                        elif key == "bookmark_pass" + s: self.bookmarks[i]["password"] = value
                        elif key == "bookmark_rcon" + s: self.bookmarks[i]["rcon"] = value
                        elif key == "bookmark_game" + s: self.bookmarks[i]["game"] = int(value) if value.lstrip("-").isdigit() else 0

    def save(self):
        lines = [
            f"ip={self.ip}",
            f"password={self.password}",
            f"rcon={self.rcon}",
            f"nickname={self.nickname}",
            f"game={self.game}",
            f"override_resolution={1 if self.override_resolution else 0}",
            f"resolution_index={self.resolution_index}",
            f"custom_width={self.custom_width}",
            f"custom_height={self.custom_height}",
        ]
        for i in range(MAX_BOOKMARKS):
            bm = self.bookmarks[i]
            if bm["name"]:
                lines.append(f"bookmark_name_{i}={bm['name']}")
                lines.append(f"bookmark_ip_{i}={bm['ip']}")
                lines.append(f"bookmark_pass_{i}={bm['password']}")
                lines.append(f"bookmark_rcon_{i}={bm['rcon']}")
                lines.append(f"bookmark_game_{i}={bm['game']}")
        with open(CFG_PATH, "w") as f:
            f.write("\n".join(lines))


class Launcher:
    def __init__(self):
        self.settings = Settings()
        self.settings.load()

        self.root = tk.Tk()
        self.root.title("MoH Launcher")
        self.root.resizable(False, False)

        frame = ttk.Frame(self.root, padding=10)
        frame.grid(sticky="nsew")

        row = 0
        self.nickname_var = tk.StringVar(value=self.settings.nickname)
        self.ip_var = tk.StringVar(value=self.settings.ip)
        self.password_var = tk.StringVar(value=self.settings.password)
        self.rcon_var = tk.StringVar(value=self.settings.rcon)

        for label, var, show in [
            ("Nickname", self.nickname_var, None),
            ("Server IP", self.ip_var, None),
            ("Password", self.password_var, "•"),
            ("RCON", self.rcon_var, "•"),
        ]:
            ttk.Label(frame, text=label, anchor="e", width=10).grid(row=row, column=0, sticky="e", padx=(0, 5), pady=2)
            e = ttk.Entry(frame, textvariable=var, width=30, show=show)
            e.grid(row=row, column=1, columnspan=4, sticky="ew", pady=2)
            row += 1

        for var in (self.nickname_var, self.ip_var, self.password_var, self.rcon_var):
            var.trace_add("write", lambda *_: self._sync_and_save())

        # Resolution
        self.override_res_var = tk.BooleanVar(value=self.settings.override_resolution)
        self.res_index_var = tk.IntVar(value=self.settings.resolution_index)
        res_frame = ttk.Frame(frame)
        res_frame.grid(row=row, column=0, columnspan=5, sticky="ew", pady=2)
        ttk.Checkbutton(res_frame, variable=self.override_res_var, command=self._toggle_resolution).grid(row=0, column=0)
        self.res_label = ttk.Label(res_frame, text="Resolution")
        self.res_combo = ttk.Combobox(res_frame, state="readonly", width=32,
                                       values=[r[0] for r in RESOLUTIONS] + ["Custom"])
        self.res_combo.current(self.settings.resolution_index)
        self.res_combo.bind("<<ComboboxSelected>>", self._on_res_change)
        self._toggle_resolution()
        row += 1

        # Custom resolution
        self.custom_frame = ttk.Frame(frame)
        self.custom_w_var = tk.StringVar(value=str(self.settings.custom_width))
        self.custom_h_var = tk.StringVar(value=str(self.settings.custom_height))
        ttk.Entry(self.custom_frame, textvariable=self.custom_w_var, width=6).grid(row=0, column=0)
        ttk.Label(self.custom_frame, text="x").grid(row=0, column=1, padx=2)
        ttk.Entry(self.custom_frame, textvariable=self.custom_h_var, width=6).grid(row=0, column=2)
        self.custom_w_var.trace_add("write", lambda *_: self._sync_and_save())
        self.custom_h_var.trace_add("write", lambda *_: self._sync_and_save())
        self._update_custom_visibility()
        row += 1

        ttk.Separator(frame, orient="horizontal").grid(row=row, column=0, columnspan=5, sticky="ew", pady=6)
        row += 1

        # Game selector
        self.game_var = tk.IntVar(value=self.settings.game)
        game_frame = ttk.Frame(frame)
        game_frame.grid(row=row, column=0, columnspan=5, sticky="ew", pady=2)
        for i, (label, _) in enumerate(GAMES):
            ttk.Radiobutton(game_frame, text=label, variable=self.game_var, value=i,
                           command=self._sync_and_save).pack(side="left", expand=True)
        row += 1

        ttk.Separator(frame, orient="horizontal").grid(row=row, column=0, columnspan=5, sticky="ew", pady=6)
        row += 1

        # Bookmarks
        ttk.Label(frame, text="Bookmarks", foreground="gray").grid(row=row, column=0, columnspan=5, sticky="w", pady=(0, 2))
        row += 1

        self.bookmark_labels = []
        for i in range(MAX_BOOKMARKS):
            bm = self.settings.bookmarks[i]
            name = bm["name"] if bm["name"] else "(empty)"
            btn = ttk.Button(frame, text=name, width=20, command=lambda idx=i: self._load_bookmark(idx))
            btn.grid(row=row, column=0, columnspan=2, sticky="ew", pady=1)
            ttk.Button(frame, text="▶", width=2, command=lambda idx=i: self._quick_connect(idx)).grid(row=row, column=2, padx=1, pady=1)
            ttk.Button(frame, text="Save", width=4, command=lambda idx=i: self._save_bookmark(idx)).grid(row=row, column=3, padx=1, pady=1)
            ttk.Button(frame, text="\U0001F5D1", width=2, command=lambda idx=i: self._delete_bookmark(idx)).grid(row=row, column=4, padx=1, pady=1)
            self.bookmark_labels.append(btn)
            row += 1

        # Connect
        connect_btn = ttk.Button(frame, text="Connect", command=self._connect)
        connect_btn.grid(row=row, column=0, columnspan=5, sticky="ew", pady=(8, 0), ipady=4)
        row += 1

        frame.columnconfigure(1, weight=1)

    def _toggle_resolution(self):
        if self.override_res_var.get():
            self.res_label.grid_remove()
            self.res_combo.grid(row=0, column=1, sticky="ew", padx=(4, 0))
        else:
            self.res_combo.grid_remove()
            self.res_label.grid(row=0, column=1, sticky="w", padx=(4, 0))
        self._update_custom_visibility()
        self._sync_and_save()

    def _on_res_change(self, _event=None):
        self._update_custom_visibility()
        self._sync_and_save()

    def _update_custom_visibility(self):
        if self.override_res_var.get() and self.res_combo.current() == len(RESOLUTIONS):
            self.custom_frame.grid(sticky="w", padx=(60, 0))
        else:
            self.custom_frame.grid_remove()

    def _sync_and_save(self):
        self.settings.nickname = self.nickname_var.get()
        self.settings.ip = self.ip_var.get()
        self.settings.password = self.password_var.get()
        self.settings.rcon = self.rcon_var.get()
        self.settings.game = self.game_var.get()
        self.settings.override_resolution = self.override_res_var.get()
        self.settings.resolution_index = self.res_combo.current()
        try: self.settings.custom_width = int(self.custom_w_var.get())
        except ValueError: pass
        try: self.settings.custom_height = int(self.custom_h_var.get())
        except ValueError: pass
        self.settings.save()

    def _load_bookmark(self, idx):
        bm = self.settings.bookmarks[idx]
        if not bm["name"]:
            return
        self.ip_var.set(bm["ip"])
        self.password_var.set(bm["password"])
        self.rcon_var.set(bm["rcon"])
        self.game_var.set(bm["game"])

    def _save_bookmark(self, idx):
        name = self.ip_var.get() or f"Bookmark {idx + 1}"
        bm = self.settings.bookmarks[idx]
        if bm["name"]:
            name = bm["name"]
        bm["name"] = name
        bm["ip"] = self.ip_var.get()
        bm["password"] = self.password_var.get()
        bm["rcon"] = self.rcon_var.get()
        bm["game"] = self.game_var.get()
        self.bookmark_labels[idx].config(text=name)
        self.settings.save()

    def _delete_bookmark(self, idx):
        self.settings.bookmarks[idx] = {"name": "", "ip": "", "password": "", "rcon": "", "game": 0}
        self.bookmark_labels[idx].config(text="(empty)")
        self.settings.save()

    def _quick_connect(self, idx):
        if not self.settings.bookmarks[idx]["name"]:
            return
        self._load_bookmark(idx)
        self._connect()

    def _connect(self):
        exe = os.path.join(SCRIPT_DIR, "moh")
        if not os.path.isfile(exe):
            messagebox.showwarning("Error", "Game executable not found. Make sure launcher.py is in the same folder as moh.")
            return

        self._sync_and_save()

        args = [exe, "+set", "fs_homepath", ".", "+set", "com_target_game", str(self.settings.game)]

        if self.settings.ip:
            args += ["+connect", self.settings.ip]
        if self.settings.password:
            args += ["+set", "password", self.settings.password]
        if self.settings.rcon:
            args += ["+set", "rconpassword", self.settings.rcon]
        if self.settings.nickname:
            args += ["+set", "name", self.settings.nickname]

        if self.settings.override_resolution:
            if self.settings.resolution_index < len(RESOLUTIONS):
                args += ["+set", "r_mode", str(RESOLUTIONS[self.settings.resolution_index][1])]
            else:
                args += ["+set", "r_mode", "-1"]
                args += ["+set", "r_customwidth", str(self.settings.custom_width)]
                args += ["+set", "r_customheight", str(self.settings.custom_height)]

        subprocess.Popen(args, cwd=SCRIPT_DIR)
        self.root.destroy()

    def run(self):
        self.root.mainloop()


if __name__ == "__main__":
    Launcher().run()
