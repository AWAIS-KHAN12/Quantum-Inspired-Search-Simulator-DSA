async function runSearch() {
    const n = Number(document.getElementById("n").value);
    const array = document.getElementById("array").value
        .trim()
        .split(/\s+/)
        .map(Number);
    const target = Number(document.getElementById("target").value);

    const output = document.getElementById("output");
    output.textContent = "Running...";

    try {
        const res = await fetch("/run", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ n, array, target })
        });

        const data = await res.json();

        if (!data.found) {
            output.textContent = "❌ Target not found";
            return;
        }

        output.textContent =
`✅ Target Found

Linear Search:
  Index: ${data.linear.index}
  Steps: ${data.linear.steps}

Binary Search:
  Index: ${data.binary.index}
  Steps: ${data.binary.steps}

Quantum-Inspired Search:
  Index: ${data.quantum.index}
  Iterations: ${data.quantum.iterations}`;
    }
    catch (err) {
        output.textContent = "Error: " + err.message;
    }
}
