FRAMERATE = 5
SIZE = 50
WIDTH = 15
HEIGHT = 10

BG_COLOR    = glua.data.color(0, 0, 0, 1)
APPLE_COLOR = glua.data.color(1, 0, 0, 1)

MOV_DIR_NONE = { x =  0, y =  0 }

MOV_DIRS = {
	["right"] = { x =  1, y =  0 },
	["left"]  = { x = -1, y =  0 },
	["up"]    = { x =  0, y = -1 },
	["down"]  = { x =  0, y =  1 }
}

local mov_queue
local snake
local length
local apple

local apple_sound
local death_sound
local move_sound

local snake_image
local apple_image

function random_pos()
	pos = {}
	pos.x = math.random(WIDTH) - 1
	pos.y = math.random(HEIGHT) - 1
	return pos
end

function pos_eq(a, b)
	return a.x == b.x and a.y == b.y
end

function hitting_snake(pos)
	for _, s in ipairs(snake) do
		if pos_eq(s, pos) then
			return true
		end
	end
	return false
end

function new_apple(pos)
	repeat
		apple = random_pos()
	until not (hitting_snake(apple) or pos_eq(apple, pos))
end

function reset()
	mov_queue = { MOV_DIR_NONE }
	snake = { random_pos() }
	length = 3
	new_apple({ -1, -1 })
end

function setup()
	apple_sound = glua.audio.load_file("apple.wav")
	death_sound = glua.audio.load_file("death.wav")
	move_sound = glua.audio.load_file("move.wav")
	snake_image = glua.texture.load_file("snake.jpg")
	apple_image = glua.texture.load_file("apple.jpg")
	glua.window.set_title("glua snake")
	glua.window.set_size(WIDTH * SIZE, HEIGHT * SIZE)
	reset()
end
setup()

function update()
	if #mov_queue > 1 then table.remove(mov_queue, 1) end
	if pos_eq(mov_queue[1], MOV_DIR_NONE) then return end

	local head = {
		x = snake[#snake].x + mov_queue[1].x,
		y = snake[#snake].y + mov_queue[1].y
	}

	if pos_eq(apple, head) then
		apple_sound:play()
		length = length + 1
		new_apple(head)
	end

	while #snake > length - 1 do
		table.remove(snake, 1)
	end

	if hitting_snake(head) or head.x < 0 or head.x == WIDTH or head.y < 0 or head.y == HEIGHT then
		death_sound:play()
		move_sound:stop()
		reset()
		return
	end

	table.insert(snake, head)
end

function glua.event.on_keydown(key)
	if key == "q" then glua.exit() end
	local mov_dir = MOV_DIRS[key]
	if #mov_queue > 5 then return end
	if mov_dir == nil then return end
	if mov_queue[#mov_queue].x * mov_dir.x ~= 0 then return end
	if mov_queue[#mov_queue].y * mov_dir.y ~= 0 then return end
	table.insert(mov_queue, mov_dir)
	move_sound:play(-1)
end

ts = 0
function glua.draw(dt)
	ts = ts + dt
	if ts < 1/FRAMERATE then return end
	ts = ts - 1/FRAMERATE
	update()
	glua.graphics.set_color(BG_COLOR)
	glua.graphics.clear()
	glua.graphics.set_color(glua.data.color(1, 0, 0, 1))
	local apple_rect = glua.data.rect(
		apple.x * SIZE,
		apple.y * SIZE,
		SIZE,
		SIZE
	)
	glua.graphics.draw_texture(apple_image, apple_rect)
	local snake_color = glua.data.color(0, 0, 0, 1)
	for i, pos in ipairs(snake) do
		snake_color.g = i / #snake / 2 + 0.5
		glua.graphics.set_color(snake_color)
		local snake_rect = glua.data.rect(
			pos.x * SIZE,
			pos.y * SIZE,
			SIZE,
			SIZE
		)
		glua.graphics.fill_rect(snake_rect)
	end
	local head_rect = glua.data.rect(snake[#snake].x * SIZE, snake[#snake].y * SIZE, SIZE, SIZE)
	glua.graphics.draw_texture(snake_image, head_rect)
	glua.graphics.show()
end

