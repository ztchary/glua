FRAMERATE = 5
SIZE = 50
WIDTH = 15
HEIGHT = 10

APPLE_SOUND = {}
DEATH_SOUND = {}

BG_COLOR    = glua.data.color(0, 0, 0, 1)
APPLE_COLOR = glua.data.color(1, 0, 0, 1)

mov_dirs = {
	NONE  = { x =  0, y =  0 },
	RIGHT = { x =  1, y =  0 },
	LEFT  = { x = -1, y =  0 },
	UP    = { x =  0, y = -1 },
	DOWN  = { x =  0, y =  1 }
}

for i = 1, 5000 do
	table.insert(APPLE_SOUND, math.sin((i-1)/25)*(1-i/5000)/3)
	table.insert(DEATH_SOUND, math.sin((i-1)/50)*(1-i/5000)/3)
end

local mov_queue
local snake
local length
local apple

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

function new_apple()
	repeat
		apple = random_pos()
	until not hitting_snake(apple)
end

function reset()
	mov_queue = { mov_dirs.NONE }
	snake = { random_pos() }
	length = 3
	new_apple()
end

function update()
	if #mov_queue > 1 then table.remove(mov_queue, 1) end
	if pos_eq(mov_queue[1], mov_dirs.NONE) then return end

	local head = {
		x = snake[#snake].x + mov_queue[1].x,
		y = snake[#snake].y + mov_queue[1].y
	}

	if pos_eq(apple, head) then
		length = length + 1
		new_apple()
		glua.audio.play_samples(APPLE_SOUND)
	end

	while #snake > length - 1 do
		table.remove(snake, 1)
	end

	if hitting_snake(head) or head.x < 0 or head.x == WIDTH or head.y < 0 or head.y == HEIGHT then
		reset()
		glua.audio.play_samples(DEATH_SOUND)
		return
	end

	table.insert(snake, head)
end

function glua.event.on_keydown(key)
	if key == "q" then
		glua.quit()
	end
	if #mov_queue > 5 then return end
	if key == "right" then
		if mov_queue[#mov_queue].x ~= 0 then return end
		table.insert(mov_queue, mov_dirs.RIGHT)
	elseif key == "left" then
		if mov_queue[#mov_queue].x ~= 0 then return end
		table.insert(mov_queue, mov_dirs.LEFT)
	elseif key == "up" then
		if mov_queue[#mov_queue].y ~= 0 then return end
		table.insert(mov_queue, mov_dirs.UP)
	elseif key == "down" then
		if mov_queue[#mov_queue].y ~= 0 then return end
		table.insert(mov_queue, mov_dirs.DOWN)
	end
end

function glua.init()
	glua.window.set_name("glua snake")
	glua.window.set_size(WIDTH * SIZE, HEIGHT * SIZE)
	reset()
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
		apple.x * SIZE + SIZE / 2,
		apple.y * SIZE + SIZE / 2,
		SIZE / 2,
		SIZE / 2
	)
	glua.graphics.fill_ellipse(apple_rect)
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
	glua.graphics.show()
end

