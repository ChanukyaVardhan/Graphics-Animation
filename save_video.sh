rm -rf video.mp4 final_video.mp4
mogrify -flip -format jpg ./frames/frame_*.ppm
ffmpeg -r 20 -i ./frames/frame_%04d.jpg video.mp4
ffmpeg -i video.mp4 -i audio.mp3 -codec copy -shortest final_video.mp4
rm -rf ./frames/*.ppm ./frames/*.jpg