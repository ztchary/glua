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

`glua.init()` init is called before initialization of the other modules

`glua.draw(dt)` draw is called every frame with deltatime

## glua.window

`glua.window.set_name(name)` sets the window name

`glua.window.set_size(width, height)` sets the window size

## glua.graphics

`glua.graphics.set_color(r, g, b, a)` sets the drawing color, values are 0.0 .. 1.0

`glua.graphics.set_color({ r, g, b, a })` color can also be supplied as an array

`glua.graphics.clear()` clears the screen with the drawing color

`glua.graphics.draw_rect(x, y, w, h)` draws a rectange with the drawing color

`glua.graphics.draw_rects({ { x, y, w, h }, { ... } ... })` draws multiple rectanges with the drawing color

`glua.graphics.fill_rect(x, y, w, h)` fills a rectange with the drawing color

`glua.graphics.fill_rects({ { x, y, w, h }, { ... } ... })` fills multiple rectanges with the drawing color

## glua.keyboard

`glua.keyboard.is_pressed(key) -> bool` queries key status, look at [keys.c][keys] for accepted values

## glua.audio

`glua.audio.play_samples({ 0.0, -1.0, 1.0 ... })` play raw samples, -1.0 to 1.0 at 44.1kHz

## glua.event

register these functions in your game, for example...

```lua
function glua.event.on_keydown(key)
    if key == "q" then
        glua.quit()
    end
end
```

currently available events include...

`glua.event.on_keydown(key)`

`glua.event.on_keyup(key)`

`glua.event.on_quit()`

[keys]: https://github.com/ztchary/glua/blob/main/src/keyboard/keys.c

