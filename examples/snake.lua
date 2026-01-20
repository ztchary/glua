FRAMERATE = 5
SIZE = 50
COLS = 15
ROWS = 10

APPLE_SOUND = {}
DEATH_SOUND = {}

for i = 1, 5000 do
	table.insert(APPLE_SOUND, math.sin((i-1)/25)*(1-i/5000)/3)
	table.insert(DEATH_SOUND, math.sin((i-1)/50)*(1-i/5000)/3)
end

function glua.init()
	glua.window.set_name("glua snake")
	glua.window.set_size(COLS * SIZE, ROWS * SIZE)
end

local mov_queue
local snake
local length
local apple_x
local apple_y

function hitting_snake(x, y)
	for _, pos in ipairs(snake) do
		if pos[1] == x and pos[2] == y then
			return true
		end
	end
	return false
end

function new_apple()
	repeat
		apple_x = math.random(COLS) - 1
		apple_y = math.random(ROWS) - 1
	until not hitting_snake(apple_x, apple_y)
end

function reset()
	mov_queue = { { 0, 0 } }
	snake = { { math.random(COLS) - 1, math.random(ROWS) - 1 } }
	length = 3
	new_apple()
end
reset()

function update()
	if #mov_queue > 1 then table.remove(mov_queue, 1) end
	if mov_queue[1][1] == 0 and mov_queue[1][2] == 0 then return end
	local head_x = snake[#snake][1] + mov_queue[1][1]
	local head_y = snake[#snake][2] + mov_queue[1][2]

	while #snake > length - 1 do
		table.remove(snake, 1)
	end

	if hitting_snake(head_x, head_y) or head_x < 0 or head_x == COLS or head_y < 0 or head_y == ROWS then
		reset()
		glua.audio.play_samples(DEATH_SOUND)
		return
	end

	table.insert(snake, { head_x, head_y })

	if head_x == apple_x and head_y == apple_y then
		length = length + 1
		new_apple()
		glua.audio.play_samples(APPLE_SOUND)
	end
end

function glua.event.on_keydown(key)
	if key == "q" then
		glua.quit()
	end
	if #mov_queue > 5 then return end
	if key == "right" then
		if mov_queue[#mov_queue][1] ~= 0 then return end
		table.insert(mov_queue, {1, 0})
	elseif key == "left" then
		if mov_queue[#mov_queue][1] ~= 0 then return end
		table.insert(mov_queue, {-1, 0})
	elseif key == "up" then
		if mov_queue[#mov_queue][2] ~= 0 then return end
		table.insert(mov_queue, {0, -1})
	elseif key == "down" then
		if mov_queue[#mov_queue][2] ~= 0 then return end
		table.insert(mov_queue, {0, 1})
	end
end

ts = 0
function glua.draw(dt)
	ts = ts + dt
	if ts > 1/FRAMERATE then
		update()
		ts = ts - 1/FRAMERATE
	end
	glua.graphics.set_color(0, 0, 0, 1)
	glua.graphics.clear()
	glua.graphics.set_color(1, 0, 0, 1)
	glua.graphics.fill_ellipse(apple_x * SIZE + SIZE / 2, apple_y * SIZE + SIZE / 2, SIZE / 2, SIZE / 2)
	for i, pos in ipairs(snake) do
		glua.graphics.set_color(0, i / #snake / 2 + 0.5, 0, 1)
		glua.graphics.fill_rect(pos[1] * SIZE, pos[2] * SIZE, SIZE, SIZE) 
	end
end

