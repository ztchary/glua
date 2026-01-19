FRAMERATE = 5
SIZE = 25 COLS = 20
ROWS = 15 

if not glua.init(COLS * SIZE, ROWS * SIZE) then
	print("glua init failed")
	glua.quit()
end

local mov_queue = { { 0, 0 } }

local snake = { { math.random(COLS) - 1, math.random(ROWS) - 1 } }
local length = 3;

local apple_x = math.random(COLS) - 1
local apple_y = math.random(ROWS) - 1

function hitting_snake(x, y)
	for i = 1, #snake do
		if snake[i][1] == x and snake[i][2] == y then
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

function update()
	if #mov_queue > 1 then table.remove(mov_queue, 1) end
	if mov_queue[1][1] == 0 and mov_queue[1][2] == 0 then return end
	local head_x = snake[#snake][1] + mov_queue[1][1]
	local head_y = snake[#snake][2] + mov_queue[1][2]

	if hitting_snake(head_x, head_y) or head_x < 0 or head_x == COLS or head_y < 0 or head_y == ROWS then
		length = 3
		snake = { { math.random(COLS) - 1, math.random(ROWS) - 1 } }
		mov_queue = { { 0, 0 } }
		new_apple()
		return
	end

	table.insert(snake, { head_x, head_y })

	if head_x == apple_x and head_y == apple_y then
		length = length + 1
		new_apple()
	end

	while #snake > length do
		table.remove(snake, 1)
	end
end

function snake_to_rects()
	local out = {}
	for i = 1, #snake do
		table.insert(out, {
			snake[i][1] * SIZE,
			snake[i][2] * SIZE,
			SIZE,
			SIZE
		})
	end
	return out
end

glua.on_key(function(keycode, keydown)
	if not keydown then return end
	if keycode == glua.keycode.KEY_Q then
		glua.quit()
	end
	if #mov_queue > 5 then return end
	if keycode == glua.keycode.KEY_RIGHT then
		if mov_queue[#mov_queue][1] ~= 0 then return end
		table.insert(mov_queue, {1, 0})
	elseif keycode == glua.keycode.KEY_LEFT then
		if mov_queue[#mov_queue][1] ~= 0 then return end
		table.insert(mov_queue, {-1, 0})
	elseif keycode == glua.keycode.KEY_UP then
		if mov_queue[#mov_queue][2] ~= 0 then return end
		table.insert(mov_queue, {0, -1})
	elseif keycode == glua.keycode.KEY_DOWN then
		if mov_queue[#mov_queue][2] ~= 0 then return end
		table.insert(mov_queue, {0, 1})
	end
end)

ts = 0
glua.set_draw(function(dt)
	ts = ts + dt
	if ts > 1/FRAMERATE then
		update()
		ts = ts - 1/FRAMERATE
	end
	glua.set_color(0, 0, 0, 1)
	glua.clear()
	glua.set_color(0, 1, 0, 1)
	glua.draw_rects(snake_to_rects())
	glua.set_color(1, 0, 0, 1)
	glua.draw_rect(apple_x * SIZE, apple_y * SIZE, SIZE, SIZE)
end)

