# Thombstone Mod Resources

## Default Death Video

To add a default death video that comes bundled with your mod:

1. **Add your video file** to this `resources` folder
2. **Name it exactly**: `default_death_video.mp4`
3. **Supported formats**: MP4, AVI, MOV, WMV (any format your system's default video player supports)

## How it works:

- **If no custom path is set** in mod settings → Uses `default_death_video.mp4` from this folder
- **If custom path is set** in mod settings → Uses your custom video file instead
- **If neither exists** → Shows warning in logs and doesn't play video

## Example structure:
```
resources/
├── README.md (this file)
└── default_death_video.mp4 (your default video)
```

## Tips:
- Keep video file size reasonable (under 50MB recommended)
- Use common video formats for better compatibility
- Test that the video plays on your system before building the mod
