# List of features

## Modes
- Vlog, tutorial, travel

## Automate thumbnail
- Pick a frame from used video clips as background frame
  - Random selection
  - An algorithm that determines the most interesting frame for use
- Find a foreground object
  - Find an interesting pose made by a person
    - Use opencv for object detection
    - Use segmentation
- Add title cards
  - [References](https://img.youtube.com/vi/L0l-GMUvPJM/0.jpg)
  
---
## Audio
### Volume
  - If volume is quiet within a certain interval
    - Speed up video and mute audio (Long duration)
      - Increase background music
    - Cut clip (Short duration - for people like me who stutters)

    - If volume is loud(containing jeers) and/or contains words
      - Play frame at normal speed
      - Lower background music

### Preprocess audios
  - Filter for clarity, filter background audio


---
## Video
### Add black frames for youtube end screen
### Lens cover transition [[Link]](https://www.youtube.com/watch?v=fzJrD7Snf4U)
- Main Use: vlog/travel videos
- Followed with a b-roll
### Image cropping algorithm
- Try to place the subject in focus
  - NOTE: Cropping is usually done on images not videos
### Enhance video