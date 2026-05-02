const express = require("express");
const { spawn } = require("child_process");
const path = require("path");

const app = express();

app.use(express.json());
app.use(express.static(path.join(__dirname, "public")));

app.post("/run", (req, res) => {
    const { n, array, target } = req.body;

    const input = `${n}\n${array.join(" ")}\n${target}\n`;

    const child = spawn(".\\main.exe", ["--json"], { shell: true });

    let stdout = "";
    let stderr = "";

    child.stdout.on("data", (data) => {
        stdout += data.toString();
    });

    child.stderr.on("data", (data) => {
        stderr += data.toString();
    });

    child.on("error", (error) => {
        res.status(500).json({ error: error.message });
    });

    child.on("close", (code) => {
        if (code !== 0) {
            res.status(500).json({ error: stderr || "Process exited with code " + code });
            return;
        }

        try {
            const jsonStart = stdout.indexOf("{");
            const cleanJson = stdout.slice(jsonStart);
            res.json(JSON.parse(cleanJson));
        } catch (e) {
            res.status(500).json({
                error: "Invalid JSON from C++ program",
                rawOutput: stdout
            });
        }
    });

    child.stdin.write(input);
    child.stdin.end();
});

app.listen(3000, () => {
    console.log("Server running on http://localhost:3000");
});
