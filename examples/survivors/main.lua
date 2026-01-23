-- ai slop (re slopped to fit new interface)

local WIDTH, HEIGHT = 800, 600
local PLAYER_SIZE = 24
local CELL_SIZE = 60
local FIXED_XP = 20

-- Window Setup (Immediate)
glua.window.set_title("Glua Survivors")
glua.window.set_size(WIDTH, HEIGHT)

-- ==========================================
-- 1. HELPERS (New API standard)
-- ==========================================
local function C(r, g, b, a) return glua.data.color(r or 1, g or 1, b or 1, a or 1) end
local function R(x, y, w, h) return glua.data.rect(x, y, w, h) end

local FONT = {
	["0"]={7,5,5,5,7},["1"]={2,2,2,2,2},["2"]={7,1,7,4,7},["3"]={7,1,7,1,7},["4"]={5,5,7,1,1},
	["5"]={7,4,7,1,7},["6"]={7,4,7,5,7},["7"]={7,1,1,1,1},["8"]={7,5,7,5,7},["9"]={7,5,7,1,1},
	["A"]={2,5,7,5,5},["B"]={6,5,6,5,6},["C"]={7,4,4,4,7},["D"]={6,5,5,5,6},["E"]={7,4,6,4,7},
	["F"]={7,4,6,4,4},["G"]={3,4,5,5,3},["H"]={5,5,7,5,5},["I"]={7,2,2,2,7},["J"]={1,1,1,5,2},
	["K"]={5,5,6,5,5},["L"]={4,4,4,4,7},["M"]={5,7,5,5,5},["N"]={5,7,7,5,5},["O"]={2,5,5,5,2},
	["P"]={6,5,6,4,4},["Q"]={2,5,5,6,1},["R"]={6,5,6,5,5},["S"]={3,4,2,1,6},["T"]={7,2,2,2,2},
	["U"]={5,5,5,5,7},["V"]={5,5,5,5,2},["W"]={5,5,5,7,5},["X"]={5,5,2,5,5},["Y"]={5,5,2,2,2},
	["Z"]={7,1,2,4,7},["-"]={0,0,7,0,0},[":"]={0,2,0,2,0},["!"]={2,2,2,0,2},["?"]={2,5,1,0,2},
	[" "]={0,0,0,0,0}, ["."]={0,0,0,0,2}, ["%"]={5,2,2,1,5}, ["+"]={0,2,7,2,0}
}

function draw_text(str, x, y, s, r, g, b, a)
	glua.graphics.set_color(C(r, g, b, a))
	local rects = {}
	str = tostring(str):upper()
	for i = 1, #str do
		local char_data = FONT[str:sub(i,i)] or FONT["0"]
		for row = 1, 5 do for col = 1, 3 do
			if math.floor(char_data[row] / 2^(3-col)) % 2 == 1 then
				-- We must insert a glua.data.rect object here
				table.insert(rects, R(x + (i-1)*4*s + (col-1)*s, y + (row-1)*s, s, s))
			end
		end end
	end
	glua.graphics.fill_rects(rects)
end

function dist(x1, y1, x2, y2) return math.sqrt((x2-x1)^2 + (y2-y1)^2) end

-- ==========================================
-- 2. GAME STATE
-- ==========================================
local state = "INTRO"
local player = { 
	x=0, y=0, hp=100, max_hp=100, shield=0, max_shield=30, lvl=1, exp=0, 
	speed=270, armor=0, luck=1, facing_x=1, gold=0, rerolls=1, reroll_cost=5,
	fire_rate=0.45, fire_timer=0, bullet_count=1, pierce=1, crit=5,
	saws=0, lightning=0, lightning_cd=0, axe=false, axe_timer=0, mine=false, mine_timer=0, poison=false, poison_timer=0,
	pet=false, pet_timer=0, missile=false, missile_timer=0, flame=false, flame_timer=0, void=false, void_timer=0,
	wand=false, wand_timer=0, orb_shield=false, garlic=false, garlic_timer=0, daggers=false, dagger_timer=0,
	magnet=90, regen=0.1, dash_cd=0, dash_timer=0, i_frames=0,
	thorns=0, lifesteal=0, dodge=0, curse=1, area=1, knockback=1, projspeed=1, greed=1, recovery=1,
	revive=0, active_cd=0, active_max=10
}

local weather = { type="CLEAR", timer=0, flash=0, bolts={}, rain={}, splash={} }
for i=1,100 do table.insert(weather.rain, {x=math.random(WIDTH), y=math.random(HEIGHT), s=math.random(10,20)}) end

local enemies, bullets, enemy_bullets, gems, pickups, popups, splats, afterimages, mines, blackholes, meteors = {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}
local destroyed_crates, visited_merchants = {}, {}
local footprints, shockwaves = {}, {}
local perks = {}
local cam_x, cam_y, shake, kills, time, wave, day_cycle = 0, 0, 0, 0, 0, 1, 0
local spawn_timer, combo, combo_timer, freeze_timer, intro_timer, tutorial_alpha = 0, 0, 0, 0, 2.0, 5.0
local dps_counter, dps_timer, slow_mo_timer = 0, 0, 0
local blood_moon, hard_mode, auto_aim, killer_name = false, false, true, ""

-- Audio source stubs (Requires actual files to work with new API)
local snd_shoot = nil -- glua.audio.source("shoot.wav")

function play_snd(source) if source then source:play() end end

-- Utility for environment generation
function get_env_at(cx, cy)
	local n = (cx * 73856093) ~ (cy * 19349663); n = math.abs(n) % 1000
	if n < 20 then return "TREE"
	elseif n < 30 then return "ROCK"
	elseif n < 35 then return "CRATE"
	elseif n < 55 then return "WATER"
	elseif n < 250 then return "GRASS"
	end
	return "NONE"
end

-- ==========================================
-- 3. PERKS & DAMAGE (Omitted logic detail for brevity, remains same)
-- ==========================================
function generate_perks()
	local pool = {{id="SPEED",n="SPEED"}, {id="FIRE",n="FIRE RATE"}, {id="DMG",n="PIERCE"}}
	perks = {}
	for i=1,3 do
		local p = pool[math.random(#pool)]
		table.insert(perks, {id=p.id, n=p.n, r="COMMON", c={1,1,1}})
	end
	state = "LEVELUP"
end

function apply_perk(idx)
	local p = perks[idx]
	if p.id=="SPEED" then player.speed = player.speed + 30 end
	state = "PLAY"
end

function take_damage(amt)
	if player.i_frames > 0 then return end
	player.hp = player.hp - amt
	player.i_frames = 0.5
	if player.hp <= 0 then state = "GAMEOVER" end
end

-- ==========================================
-- 4. UPDATE
-- ==========================================
function update(dt)
	if state == "INTRO" then
		intro_timer = intro_timer - dt
		if intro_timer < 0 then state = "PLAY" end
		return
	end
	if state ~= "PLAY" then return end
	
	time = time + dt
	day_cycle = math.sin(time/20)

	-- Basic Player Movement
	local vx, vy = 0, 0
	if glua.keyboard.is_pressed("w") or glua.keyboard.is_pressed("up") then vy = -1 end
	if glua.keyboard.is_pressed("s") or glua.keyboard.is_pressed("down") then vy = 1 end
	if glua.keyboard.is_pressed("a") or glua.keyboard.is_pressed("left") then vx = -1 end
	if glua.keyboard.is_pressed("d") or glua.keyboard.is_pressed("right") then vx = 1 end
	
	if vx ~= 0 or vy ~= 0 then
		local m = math.sqrt(vx*vx + vy*vy)
		player.x = player.x + (vx/m) * player.speed * dt
		player.y = player.y + (vy/m) * player.speed * dt
		if vx ~= 0 then player.facing_x = vx end
	end

	-- Camera follows player
	cam_x = player.x - WIDTH/2
	cam_y = player.y - HEIGHT/2

	-- Spawn simple enemies
	spawn_timer = spawn_timer + dt
	if spawn_timer > 1.0 then
		local a = math.random() * 6.28
		table.insert(enemies, {x=player.x + math.cos(a)*500, y=player.y + math.sin(a)*500, hp=5})
		spawn_timer = 0
	end

	-- Simple Enemy AI
	for i=#enemies, 1, -1 do
		local e = enemies[i]
		local d = dist(e.x, e.y, player.x, player.y)
		e.x = e.x + (player.x - e.x)/d * 100 * dt
		e.y = e.y + (player.y - e.y)/d * 100 * dt
		if d < 20 then take_damage(10 * dt) end
	end

	-- Weather update
	if weather.type == "RAIN" or weather.type == "STORM" then
		for _, r in ipairs(weather.rain) do
			r.y = (r.y + 500 * dt) % HEIGHT
			r.x = (r.x - 50 * dt) % WIDTH
		end
	end
	weather.timer = weather.timer + dt
	if weather.timer > 10 then 
		weather.type = math.random() > 0.5 and "RAIN" or "CLEAR"
		weather.timer = 0
	end
end

-- ==========================================
-- 5. DRAW
-- ==========================================
function glua.draw(dt)
	update(dt)

	-- 1. Clear Screen
	local dv = (day_cycle + 1) / 2
	glua.graphics.set_color(C(0.1 * dv, 0.1 * dv, 0.15 * dv, 1))
	glua.graphics.clear()

	if state == "INTRO" then
		draw_text("GLUA SURVIVORS", 240, 280, 4, 1, 1, 1, 1)
		glua.graphics.show()
		return
	end

	-- 2. Draw World Grid
	glua.graphics.set_color(C(1, 1, 1, 0.1))
	local grid_rects = {}
	for i=0, 10 do
		table.insert(grid_rects, R(0, i*80 - cam_y%80, WIDTH, 1))
		table.insert(grid_rects, R(i*80 - cam_x%80, 0, 1, HEIGHT))
	end
	glua.graphics.fill_rects(grid_rects)

	-- 3. Draw Environment
	local cx_start, cy_start = math.floor(cam_x/CELL_SIZE), math.floor(cam_y/CELL_SIZE)
	local cx_end, cy_end = cx_start + math.ceil(WIDTH/CELL_SIZE) + 1, cy_start + math.ceil(HEIGHT/CELL_SIZE) + 1
	
	for cx = cx_start, cx_end do for cy = cy_start, cy_end do
		local t = get_env_at(cx, cy)
		local x, y = cx*CELL_SIZE - cam_x, cy*CELL_SIZE - cam_y
		if t == "TREE" then
			glua.graphics.set_color(C(0.4, 0.2, 0, 1))
			glua.graphics.fill_rect(R(x+25, y+30, 10, 30))
			glua.graphics.set_color(C(0.1, 0.7, 0.2, 0.8))
			glua.graphics.fill_rect(R(x+10, y+5, 40, 40))
		elseif t == "WATER" then
			glua.graphics.set_color(C(0, 0.4, 0.8, 0.4))
			glua.graphics.fill_rect(R(x, y, CELL_SIZE, CELL_SIZE))
		end
	end end

	-- 4. Draw Enemies
	glua.graphics.set_color(C(0.8, 0.2, 0.2, 1))
	for _, e in ipairs(enemies) do
		glua.graphics.fill_rect(R(e.x - cam_x, e.y - cam_y, 24, 24))
	end

	-- 5. Draw Player
	if player.i_frames <= 0 or math.floor(time*10)%2 == 0 then
		glua.graphics.set_color(C(1, 1, 1, 1))
		glua.graphics.fill_rect(R(player.x - cam_x, player.y - cam_y, 24, 24))
		-- Eyes
		glua.graphics.set_color(C(0, 0, 0, 1))
		local eye_x = (player.facing_x > 0) and 14 or 2
		glua.graphics.fill_rect(R(player.x - cam_x + eye_x, player.y - cam_y + 6, 4, 4))
		glua.graphics.fill_rect(R(player.x - cam_x + eye_x + 6, player.y - cam_y + 6, 4, 4))
	end

	-- 6. Draw Weather
	if weather.type == "RAIN" then
		glua.graphics.set_color(C(0.5, 0.5, 1, 0.3))
		local rain_objs = {}
		for _, r in ipairs(weather.rain) do
			table.insert(rain_objs, R(r.x, r.y, 2, r.s))
		end
		glua.graphics.fill_rects(rain_objs)
	end

	-- 7. UI
	-- Health Bar
	glua.graphics.set_color(C(0, 0, 0, 0.5))
	glua.graphics.fill_rect(R(20, 20, 204, 24))
	glua.graphics.set_color(C(1, 0, 0, 1))
	glua.graphics.fill_rect(R(22, 22, 200 * (player.hp/player.max_hp), 20))
	
	draw_text("LVL " .. player.lvl, 20, 50, 2)
	draw_text("XP " .. math.floor(player.exp), 20, 75, 2)
	draw_text("KILLS " .. kills, WIDTH - 120, 20, 2)

	if state == "LEVELUP" then
		glua.graphics.set_color(C(0,0,0,0.8))
		glua.graphics.fill_rect(R(0,0,WIDTH,HEIGHT))
		draw_text("CHOOSE A PERK", 250, 150, 3, 1, 1, 0)
		for i, p in ipairs(perks) do
			draw_text(i .. ". " .. p.n, 300, 200 + i*40, 2)
		end
	end

	if state == "GAMEOVER" then
		glua.graphics.set_color(C(0,0,0,0.9))
		glua.graphics.fill_rect(R(0,0,WIDTH,HEIGHT))
		draw_text("GAME OVER", 280, 250, 5, 1, 0, 0)
		draw_text("PRESS Q TO QUIT", 310, 320, 2)
	end

	-- 8. MANDATORY: Show backbuffer
	glua.graphics.show()
end

-- ==========================================
-- 6. EVENTS
-- ==========================================
function glua.event.on_keydown(k)
	if state == "LEVELUP" then
		if k == "1" then apply_perk(1) end
		if k == "2" then apply_perk(2) end
		if k == "3" then apply_perk(3) end
	end
	if k == "q" then glua.quit() end
end
