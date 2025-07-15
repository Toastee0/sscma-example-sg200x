## Project Framework
React + TypeScript + TailwindCSS + Vite + Antd-Mobile + Zustand

## Project Setup
- Development environment:
```
npm install
npm run dev
```

- Testing environment build:
```
npm install
npm run staging
```

- Production environment build:
```
npm install
npm run build
```

## Deployment to Device
### Step 1: Transfer compiled dist folder to device
```
1. sftp recamera@192.168.42.1
   IP address is the device IP, fixed as 192.168.42.1 when connected via USB

2. put -r dist
   dist is the local directory after project compilation (build folder)
```

### Step 2: Copy dist directory to www directory on device
```
1. cd /usr/share/supervisor/

2. sudo rm -rf www/
   If there's an existing www directory under supervisor, delete it first

3. sudo cp /home/recamera/dist/ www -r

4. sudo reboot
```

Access via http://192.168.42.1


### Project Directory Structure

```
├── App.tsx
├── api                     // API related
├── assets
│   ├── images
│   └── style
├── components              // Global components
├── enum                    // Global enumeration definitions
├── layout                  // Mobile and PC layout adaptation
│   ├── mobile              // Mobile layout
│   ├── pc                  // PC layout
│   └── index               // Responsive switching between mobile and PC layouts
├── main.tsx
├── router                  // Routing
├── store                   // Global state management
│   ├── config              // Device configuration related
│   ├── platform            // SenseCraft platform related
│   └── user                // Device account and password related
├── utils                   // Tools and methods
├── views                   // Pages
│   ├── dashboard           // Node-RED dashboard page wrapper
│   ├── login               // SenseCraft platform related - Device login page, same level as other route pages
│   ├── network             // SenseCraft platform related - Device network configuration
│   ├── overview            // SenseCraft platform related - Device overview
│   ├── power               // SenseCraft platform related - Device power control
│   ├── security            // SenseCraft platform related - Device security
│   ├── system              // SenseCraft platform related - Device system information
│   ├── terminal            // SenseCraft platform related - Device terminal
│   └── workspace           // SenseCraft platform related - SenseCraft workspace
```
