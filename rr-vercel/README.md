# Round Robin CPU Scheduling Simulator (C++ + WebAssembly)

The Round Robin scheduling engine is implemented in `rr.cpp` (C++), compiled to `public/rr.wasm`, and executed in the browser through WebAssembly. The user interface is static HTML/JavaScript and is deployable on Vercel.

## Deploy to Vercel

1. Create a GitHub repository and upload all files/folders from this project, preserving the `public/rr.wasm` path.
2. Sign in to https://vercel.com/ and choose **Add New → Project**.
3. Import the GitHub repository.
4. Framework Preset: **Other**. Build Command: leave blank. Output Directory: `.`. Install Command: leave blank.
5. Click **Deploy**. Vercel will provide a public `https://<project-name>.vercel.app` URL.
6. To change the URL, open Project → Settings → Domains, and add an available project domain or a domain you own.

No backend server is needed. The C++ WebAssembly module runs in the visitor's browser.

## Run locally

Open `index.html` directly in a browser only if the browser allows the WASM file fetch; recommended:

```bash
python -m http.server 3000
```

Then visit http://localhost:3000.

## C++ source / rebuild

`rr.cpp` contains the scheduling engine. It was compiled with:

```bash
clang++ --target=wasm32-unknown-unknown -O2 -nostdlib rr.cpp -o public/rr.wasm \
  -Wl,--no-entry -Wl,--export=run_round_robin -Wl,--export-memory \
  -Wl,--initial-memory=2097152 -Wl,--max-memory=2097152 -Wl,--allow-undefined
```
