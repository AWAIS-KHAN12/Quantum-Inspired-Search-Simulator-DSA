const express = require("express");
const { exec } = require("child_process");
const path = require("path");

const app = express();

app.use(express.json());
app.use(express.static(path.join(__dirname, "public")));

app.post("/run", (req, res) => {
    const { n, array, target } = req.body;

    // Build input exactly as C++ expects
    const input = `${n}\n${array.join(" ")}\n${target}\n`;

    exec(".\\main.exe --json", { input, timeout: 5000 }, (error, stdout, stderr) => {
        if (error) {
            res.status(500).json({ error: error.message });
            return;
        }

        try {
            // Extract only JSON (ignore banner text)
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
});

app.listen(3000, () => {
    console.log("Server running on http://localhost:3000");
});
