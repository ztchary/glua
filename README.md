# glua (game lua i think)

i dont remember where the name came from

just trying out the lua api, and getting better with sdl2

## check out examples

./glua snage.lua

# docs

will change violently and rapidly

## glua

`glua.quit()` quits the application

these functions should be defined in your code...

`glua.draw(dt)` draw is called every frame with deltatime

## glua.data

`glua.data.rect(x, y, w, h)` create a rectangle object

`glua.data.rect(x, y, rx, ry)` rect is also used for ellipses

`glua.data.color(r, g, b, a)` create a color object (values range from 0.0 to 1.0)

## glua.window

`glua.window.set_name(name)` sets the window name

`glua.window.set_size(width, height)` sets the window size

## glua.texture

`glua.texture.load_file(path)` loads an image file as a texture

`texture:get_size()` returns the width and height of the image

## glua.graphics

`glua.graphics.set_color(color)` sets the drawing color

`glua.graphics.clear()` clears the screen with the drawing color

`glua.graphics.show()` displays the rendered image on the screen (required)

`glua.graphics.draw_rect(rect)` draws a rectangle with the drawing color

`glua.graphics.draw_rects({ rect, ... })` draws multiple rectangles with the drawing color

`glua.graphics.fill_rect(rect)` fills a rectangle with the drawing color

`glua.graphics.fill_rects({ rect, ... })` fills multiple rectangles with the drawing color

`glua.graphics.draw_ellipse(rect)` draws an ellipse with the drawing color, rect.w and rect.h are used as x and y radius

`glua.graphics.draw_ellipses({ rect, ... })` draws multiple ellipses with the drawing color

`glua.graphics.fill_ellipse(rect)` fills an ellipse with the drawing color

`glua.graphics.fill_ellipses({ rect, ... })` fills multiple ellipses with the drawing color

`glua.graphics.draw_texture(texture, rect)` renders a texture to the screen

## glua.keyboard

`glua.keyboard.is_pressed(key) -> bool` queries key status, look at [keys.c][keys] for accepted values

## glua.audio

`glua.audio.source(path)` load new audio source from file

`source:play()` play or resume an audio source

`source:play(n)` play an audio source, looped n times

`source:pause()` pause audio source

`source:stop()` stop playing audio, running play will restart the audio

`source:set_volume(vol)` set audio source volume (values range from 0.0 to 1.0)

## glua.event

register these functions in your game, for example...

```lua
function glua.event.on_keydown(key)
    if key == "q" then
        glua.exit()
    end
end
```

currently available events include...

`glua.event.on_keydown(key)`

`glua.event.on_keyup(key)`

`glua.event.on_exit()`

[keys]: https://github.com/ztchary/glua/blob/main/src/keyboard/keys.c

