getgenv().AimPart = "Head" -- For R15 Games: {UpperTorso, LowerTorso, HumanoidRootPart, Head} | For R6 Games: {Head, Torso, HumanoidRootPart}
getgenv().AimlockKey = "q"
getgenv().AimRadius = 30 -- How far away from someones character you want to lock on at
getgenv().ThirdPerson = true 
getgenv().FirstPerson = true
getgenv().TeamCheck = false -- Check if Target is on your Team (True means it wont lock onto your teamates, false is vice versa) (Set it to false if there are no teams)
getgenv().PredictMovement = true -- Predicts if they are moving in fast velocity (like jumping) so the aimbot will go a bit faster to match their speed 
getgenv().PredictionVelocity = 7
getgenv().OldAimPart = "HumanoidRootPart"
getgenv().CheckIfJumped = true
getgenv().Multiplier = -0.27


for _, v in pairs(game.Players.LocalPlayer.Character:GetChildren()) do
    if v:IsA("Script") and v.Name ~= "Health" and v.Name ~= "Sound" and v:FindFirstChild("LocalScript") then
        v:Destroy()
    end
end
game.Players.LocalPlayer.CharacterAdded:Connect(function(char)
    repeat
        wait()
    until game.Players.LocalPlayer.Character
    char.ChildAdded:Connect(function(child)
        if child:IsA("Script") then 
            wait(0.1)
            if child:FindFirstChild("LocalScript") then
                child.LocalScript:FireServer()
            end
        end
    end)
end)

repeat wait() until game:IsLoaded()

    getgenv().Aimlock = false

    local Players, Uis, RService, SGui = game:GetService"Players", game:GetService"UserInputService", game:GetService"RunService", game:GetService"StarterGui";
    local Client, Mouse, Camera, CF, RNew, Vec3, Vec2 = Players.LocalPlayer, Players.LocalPlayer:GetMouse(), workspace.CurrentCamera, CFrame.new, Ray.new, Vector3.new, Vector2.new;
    local MousePressed, CanNotify = false, false;
    local AimlockTarget;
    local OldPre;

    getgenv().CiazwareUniversalAimbotLoaded = true

    getgenv().WorldToViewportPoint = function(P)
        return Camera:WorldToViewportPoint(P)
    end

    getgenv().WorldToScreenPoint = function(P)
        return Camera.WorldToScreenPoint(Camera, P)
    end

    getgenv().GetObscuringObjects = function(T)
        if T and T:FindFirstChild(getgenv().AimPart) and Client and Client.Character:FindFirstChild("Head") then 
            local RayPos = workspace:FindPartOnRay(RNew(
                T[getgenv().AimPart].Position, Client.Character.Head.Position)
            )
            if RayPos then return RayPos:IsDescendantOf(T) end
        end
    end

    getgenv().GetNearestTarget = function()
        -- Credits to whoever made this, i didnt make it, and my own mouse2plr function kinda sucks
        local players = {}
        local PLAYER_HOLD  = {}
        local DISTANCES = {}
        for i, v in pairs(Players:GetPlayers()) do
            if v ~= Client then
                table.insert(players, v)
            end
        end
        for i, v in pairs(players) do
            if v.Character ~= nil then
                local AIM = v.Character:FindFirstChild("Head")
                if getgenv().TeamCheck == true and v.Team ~= Client.Team then
                    local DISTANCE = (v.Character:FindFirstChild("Head").Position - game.Workspace.CurrentCamera.CFrame.p).magnitude
                    local RAY = Ray.new(game.Workspace.CurrentCamera.CFrame.p, (Mouse.Hit.p - game.Workspace.CurrentCamera.CFrame.p).unit * DISTANCE)
                    local HIT,POS = game.Workspace:FindPartOnRay(RAY, game.Workspace)
                    local DIFF = math.floor((POS - AIM.Position).magnitude)
                    PLAYER_HOLD[v.Name .. i] = {}
                    PLAYER_HOLD[v.Name .. i].dist= DISTANCE
                    PLAYER_HOLD[v.Name .. i].plr = v
                    PLAYER_HOLD[v.Name .. i].diff = DIFF
                    table.insert(DISTANCES, DIFF)
                elseif getgenv().TeamCheck == false and v.Team == Client.Team then 
                    local DISTANCE = (v.Character:FindFirstChild("Head").Position - game.Workspace.CurrentCamera.CFrame.p).magnitude
                    local RAY = Ray.new(game.Workspace.CurrentCamera.CFrame.p, (Mouse.Hit.p - game.Workspace.CurrentCamera.CFrame.p).unit * DISTANCE)
                    local HIT,POS = game.Workspace:FindPartOnRay(RAY, game.Workspace)
                    local DIFF = math.floor((POS - AIM.Position).magnitude)
                    PLAYER_HOLD[v.Name .. i] = {}
                    PLAYER_HOLD[v.Name .. i].dist= DISTANCE
                    PLAYER_HOLD[v.Name .. i].plr = v
                    PLAYER_HOLD[v.Name .. i].diff = DIFF
                    table.insert(DISTANCES, DIFF)
                end
            end
        end
        
        if unpack(DISTANCES) == nil then
            return nil
        end
        
        local L_DISTANCE = math.floor(math.min(unpack(DISTANCES)))
        if L_DISTANCE > getgenv().AimRadius then
            return nil
        end
        
        for i, v in pairs(PLAYER_HOLD) do
            if v.diff == L_DISTANCE then
                return v.plr
            end
        end
        return nil
    end

    Mouse.KeyDown:Connect(function(a)
        if not (Uis:GetFocusedTextBox()) then 
            if a == AimlockKey and AimlockTarget == nil then
                pcall(function()
                    if MousePressed ~= true then MousePressed = true end 
                    local Target;Target = GetNearestTarget()
                    if Target ~= nil then 
                        AimlockTarget = Target
                    end
                end)
            elseif a == AimlockKey and AimlockTarget ~= nil then
                if AimlockTarget ~= nil then AimlockTarget = nil end
                if MousePressed ~= false then 
                    MousePressed = false 
                end
            end
        end
    end)
    RService.RenderStepped:Connect(function()
        local AimPartOld = getgenv().OldAimPart
        if getgenv().ThirdPerson == true and getgenv().FirstPerson == true then 
            if (Camera.Focus.p - Camera.CoordinateFrame.p).Magnitude > 1 or (Camera.Focus.p - Camera.CoordinateFrame.p).Magnitude <= 1 then 
                CanNotify = true 
            else 
                CanNotify = false 
            end
        elseif getgenv().ThirdPerson == true and getgenv().FirstPerson == false then 
            if (Camera.Focus.p - Camera.CoordinateFrame.p).Magnitude > 1 then 
                CanNotify = true 
            else 
                CanNotify = false 
            end
        elseif getgenv().ThirdPerson == false and getgenv().FirstPerson == true then 
            if (Camera.Focus.p - Camera.CoordinateFrame.p).Magnitude <= 1 then 
                CanNotify = true 
            else 
                CanNotify = false 
            end
        end
        if getgenv().Aimlock == true and MousePressed == true then 
            if AimlockTarget and AimlockTarget.Character and AimlockTarget.Character:FindFirstChild(getgenv().AimPart) then 
                if getgenv().FirstPerson == true then
                    if CanNotify == true then
                        if getgenv().PredictMovement == true then 
                            Camera.CFrame = CF(Camera.CFrame.p, AimlockTarget.Character[getgenv().AimPart].Position + AimlockTarget.Character[getgenv().AimPart].Velocity/PredictionVelocity)
                        elseif getgenv().PredictMovement == false then 
                            Camera.CFrame = CF(Camera.CFrame.p, AimlockTarget.Character[getgenv().AimPart].Position)
                        end
                    end
                elseif getgenv().ThirdPerson == true then 
                    if CanNotify == true then
                        if getgenv().PredictMovement == true then 
                            Camera.CFrame = CF(Camera.CFrame.p, AimlockTarget.Character[getgenv().AimPart].Position + AimlockTarget.Character[getgenv().AimPart].Velocity/PredictionVelocity)
                        elseif getgenv().PredictMovement == false then 
                            Camera.CFrame = CF(Camera.CFrame.p, AimlockTarget.Character[getgenv().AimPart].Position)
                        end
                    end 
                end
            end
        end
        if getgenv().CheckIfJumped == true then
            if AimlockTarget.Character.Humanoid.FloorMaterial == Enum.Material.Air and AimlockTarget.Character.Humanoid.Jump == true then
                getgenv().AimPart = "RightLowerLeg"
            else
                getgenv().AimPart = AimPartOld
            end
        end
    end)







superhuman = false
	plr = game.Players.LocalPlayer
	mouse = plr:GetMouse()
	mouse.KeyDown:connect(function(key)
		if key == _G.Lol and superhuman == false then
			superhuman = true
			game.Players.LocalPlayer.Character.Humanoid.Name = "Humz"
			game.Players.LocalPlayer.Character.Humz.WalkSpeed = _G.Speeed
			game.Players.LocalPlayer.Character.Humz.JumpPower = 50
		elseif key == _G.Lol and superhuman == true then
			superhuman = false
			game.Players.LocalPlayer.Character.Humz.WalkSpeed = 16
			game.Players.LocalPlayer.Character.Humz.JumpPower = 50
			game.Players.LocalPlayer.Character.Humz.Name = "Humanoid"
		end
	end)

loadstring(game:HttpGet("https://pastebin.com/raw/D78FSqvR"))()
local ezlib = {};

------------------------------------------------------------------------------
-- Stores the functions and variables essential for the UI to work.

local coreVars = {};	-- Stores essential vars
local coreFuncs = {};	-- Stores essential funcs
local coreGUIFuncs = {};	-- Stores functions that create the actual GUI

------------------------------------------------------------------------------
-- coreVars definitions

coreVars.navDebounce = true;
coreVars.usingSlider = false;
coreVars.elementX = 300
coreVars.sliderUsed = nil;

-- Used for keybind ui.
coreVars.awaitingInput = nil;	-- Stores the keybind that is requesting input from the user.
game:GetService("UserInputService").InputBegan:Connect(function(input)
	if coreVars.awaitingInput then
		coreVars.awaitingInput.button.Text = input.KeyCode.Name;
		coreVars.awaitingInput = nil;
	end
end)
coreVars.colors = {
	Primary = Color3.fromRGB(25, 25, 25),
	Secondary = Color3.fromRGB(30, 30, 30),
	Tertiary = Color3.fromRGB(35, 35, 35),
	Quaternary = Color3.fromRGB(40, 40, 40)
};

if _G.EzHubTheme then

	local p = _G.EzHubTheme["Primary"];
	local s = _G.EzHubTheme["Secondary"];
	local t = _G.EzHubTheme["Tertiary"];
	local q = _G.EzHubTheme["Quaternary"];

	coreVars.colors = {
		Primary = Color3.fromRGB(p[1], p[2], p[3]),
		Secondary = Color3.fromRGB(s[1], s[2], s[3]),
		Tertiary = Color3.fromRGB(t[1], t[2], t[3]),
		Quaternary = Color3.fromRGB(q[1], q[2], q[3])
	}

end

------------------------------------------------------------------------------
-- Corefuncs definitions

coreFuncs.addInstance = function(instance, properties)
	if instance and type(properties) == "table" then
		local ins = Instance.new(tostring(instance));
		for i,v in pairs(properties or {}) do
			ins[i] = v;
		end
		return ins;
	else
		error("Invalid input for addInstance function.");
	end
end

coreFuncs.roundify = function(element, radius)
	coreFuncs.addInstance("UICorner", {
		CornerRadius =  radius and UDim.new(0, radius) or UDim.new(0,4),
		Parent = element
	});
end

coreFuncs.isUsingSlider = function()
	return coreVars.usingSlider;
end

coreFuncs.dragifyLib = function(mainFrame)
	local dragging;
	local dragInput;
	local dragStart;
	local startPos;

	local function update(input)
		if coreFuncs.isUsingSlider() then return end
		Delta = input.Position - dragStart;
		Position = UDim2.new(startPos.X.Scale, startPos.X.Offset + Delta.X, startPos.Y.Scale, startPos.Y.Offset + Delta.Y);
		game:GetService("TweenService"):Create(mainFrame, TweenInfo.new(.15), {Position = Position}):Play();
	end

	mainFrame.InputBegan:Connect(function(input)
		if coreFuncs.isUsingSlider() then return end
		if input.UserInputType == Enum.UserInputType.MouseButton1 or input.UserInputType == Enum.UserInputType.Touch then
			dragging = true;
			dragStart = input.Position;
			startPos = mainFrame.Position;

			input.Changed:Connect(function()
				if input.UserInputState == Enum.UserInputState.End then
					dragging = false;
				end
			end)
		end
	end)

	mainFrame.InputChanged:Connect(function(input)
		if coreFuncs.isUsingSlider() then return end
		if (input.UserInputType == Enum.UserInputType.MouseMovement or input.UserInputType == Enum.UserInputType.Touch) then
			dragInput = input;
		end
	end)

	game:GetService("UserInputService").InputChanged:Connect(function(input)
		if input == dragInput and dragging then
			update(input);
		end
	end)
end

-- Function for rotating instances. More info in main Ez Hub file
coreFuncs.rotateInstanceBy = function(instance, rotationAngle, delay, isCounterClockwise)
	for i = 1, rotationAngle / 40 do
		local tween = game:GetService("TweenService"):Create(instance, TweenInfo.new(delay or 0.03, Enum.EasingStyle.Linear), {Rotation = instance.Rotation + (isCounterClockwise and -40 or 40)});
		tween:Play();
		tween.Completed:Wait();
	end
end

-- Handles the navigation tab opening and closing
coreFuncs.handleNavLib = function(frame, nav, close, activeFrame)
	if coreVars.navDebounce then
		coreVars.navDebounce = false;

		coroutine.wrap(function()
			if frame.Position ~= UDim2.new(-0.5, 0, 0.108, 0) then
				frame:TweenPosition(UDim2.new(-0.5, 0, 0.108, 0),Enum.EasingDirection.Out, Enum.EasingStyle.Quad, 0.3, true);	-- Hide
				if not activeFrame then return end
				activeFrame:TweenPosition(activeFrame.Position - UDim2.new(0,frame.Size.X.Offset + 20,0,0), Enum.EasingDirection.Out, Enum.EasingStyle.Quad, 0.3, true);
			else
				frame:TweenPosition(UDim2.new(0, 0,0.108, 0),Enum.EasingDirection.Out, Enum.EasingStyle.Quad, 0.3, true);		-- Show
				if not activeFrame then return end
				activeFrame:TweenPosition(activeFrame.Position + UDim2.new(0,frame.Size.X.Offset + 20,0,0), Enum.EasingDirection.Out, Enum.EasingStyle.Quad, 0.3, true);
			end
		end)();

		if nav.Visible then

			coreFuncs.rotateInstanceBy(nav, 180);
			nav.Visible = false;
			close.Rotation = nav.Rotation;
			close.Visible = true;
			coreFuncs.rotateInstanceBy(close, 200);

		else

			coreFuncs.rotateInstanceBy(close, 180, nil, true);
			nav.Visible = true;
			nav.Rotation = close.Rotation;
			close.Visible = false;
			coreFuncs.rotateInstanceBy(nav, 200, nil, true);

		end

		coreVars.navDebounce = true;
	end
end

coreFuncs.applyFrameResizingLib = function(scrollingframe)
	local calc = scrollingframe:FindFirstChild("UIGridLayout") or scrollingframe:FindFirstChild("UIListLayout");
	local function update()
		pcall(function()
			local cS = calc.AbsoluteContentSize;
			scrollingframe.CanvasSize = UDim2.new(0,scrollingframe.Size.X,0,cS.Y + 30);
		end)
	end
	calc.Changed:Connect(update);
	update();
end

------------------------------------------------------------------------------
-- coreGUIFuncs definitions

coreGUIFuncs.newCreateGUI = function(name, pos, parent, colors)

	local screenGui = coreFuncs.addInstance("ScreenGui", {
		["Parent"] = parent,
		["Name"] = "EzLib"
	});

	local window = coreFuncs.addInstance("Frame", {
		["Position"] = pos,
		["Name"] = "MainFrame",
		["AnchorPoint"] = Vector2.new(0.5, 0.5),
		["Size"] = UDim2.new(0, 350, 0, 270),
		["ClipsDescendants"] = true,
		["BorderSizePixel"] = 0,
		["BackgroundColor3"] = colors.Primary,
		["Parent"] = screenGui
	});

	coreFuncs.dragifyLib(window);
	coreFuncs.roundify(window);

	local topFrame = coreFuncs.addInstance("Frame", {
		["Name"] = "TopFrame",
		["BackgroundColor3"] = colors.Tertiary,
		["BorderSizePixel"] = 0,
		["Size"] = UDim2.new(1, 0, 0, 34),
		["ZIndex"] = 3,
		["Parent"] = window
	});

	coreFuncs.roundify(topFrame);

	coreFuncs.addInstance("Frame", {
		["Size"] = UDim2.new(1, 0, 0, 4),
		["AnchorPoint"] = Vector2.new(0, 1),
		["Position"] = UDim2.new(0, 0, 1, 0),
		["BackgroundColor3"] = colors.Tertiary,
		["ZIndex"] = 4,
		["BorderSizePixel"] = 0,
		["Parent"] = topFrame
	})

	coreFuncs.addInstance("TextLabel", {
		["Name"] = "UIName",
		["BackgroundTransparency"] = 1,
		["TextColor3"] = Color3.fromRGB(255,255,255),
		["AnchorPoint"] = Vector2.new(1,0.5),
		["Position"] = UDim2.new(1,-10,0.5,0),
		["Text"] = name,
		["Size"] = UDim2.new(0, 200, 0, 30),
		["TextWrapped"] = true,
		["ZIndex"] = 3,
		["TextXAlignment"] = Enum.TextXAlignment.Right,
		["Parent"] = topFrame
	});

	local closeNav = coreFuncs.addInstance("ImageButton", {
		["Name"] = "CloseNavButton",
		["AnchorPoint"] = Vector2.new(0.5, 0.5),
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["BorderSizePixel"] = 0,
		["Position"] = UDim2.new(0.05, 0, 0.51, 0),
		["Size"] = UDim2.new(0, 20, 0, 20),
		["Visible"] = false,
		["ZIndex"] = 4,
		["Image"] = "http://www.roblox.com/asset/?id=5969992570",
		["Parent"] = topFrame
	});

	local openNav = coreFuncs.addInstance("ImageButton", {
		["Name"] = "NavButton",
		["AnchorPoint"] = Vector2.new(0.5, 0.5),
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["BorderSizePixel"] = 0,
		["Position"] = UDim2.new(0.05, 0, 0.51, 0),
		["Size"] = UDim2.new(0, 28, 0, 28),
		["ZIndex"] = 4,
		["Image"] = "http://www.roblox.com/asset/?id=5942241281",
		["Parent"] = topFrame
	});

	local navFrame = coreFuncs.addInstance("ScrollingFrame", {
		["Name"] = "NavFrame",
		["Active"] = true,
		["BackgroundColor3"] = colors.Tertiary,
		["BorderSizePixel"] = 0,
		["Position"] = UDim2.new(-0.5, 0, 0.108000003, 0),
		["ScrollBarThickness"] = 0,
		["Size"] = UDim2.new(0, 130, 1, -34),
		["ZIndex"] = 2,
		["CanvasSize"] = UDim2.new(0, 0, 0, 0),
		["Parent"] = window
	});

	coreFuncs.addInstance("UIGridLayout", {
		["HorizontalAlignment"] = Enum.HorizontalAlignment.Center,
		["SortOrder"] = Enum.SortOrder.LayoutOrder,
		["CellPadding"] = UDim2.new(0, 5, 0, 7),
		["CellSize"] = UDim2.new(0, 95, 0, 25),
		["Parent"] = navFrame
	});

	coreFuncs.addInstance("UIPadding", {
		["PaddingBottom"] = UDim.new(0, 15),
		["PaddingTop"] = UDim.new(0, 20),
		["Parent"] = navFrame
	});

	coreFuncs.applyFrameResizingLib(navFrame);
	return ({["opennav"] = openNav, ["closenav"] = closeNav, ["navframe"] = navFrame, ["screengui"] = screenGui, ["window"] = window});

end

coreGUIFuncs.newTab = function(libInstance, name, colors)

	local button = coreFuncs.addInstance("TextButton", {
		["Text"] = tostring(name),
		["Name"] = tostring(name),
		["BorderSizePixel"] = 0,
		["BackgroundColor3"] = colors.Primary,
		["TextColor3"] = Color3.fromRGB(255,255,255),
		["ZIndex"] = 3,
		["Parent"] = libInstance.MainFrame.NavFrame
	});

	local blueLine = coreFuncs.addInstance("Frame", {
		["BackgroundColor3"] = colors.Quaternary,
		["Size"] = UDim2.new(0, 2, 1, 0),
		["ZIndex"] = 4,
		["Parent"] = button
	});

	coreFuncs.roundify(blueLine, 8);

	local window = coreFuncs.addInstance("ScrollingFrame", {
		["Name"] = tostring(name),
		["BorderSizePixel"] = 0,
		["BackgroundTransparency"] = 1,
		["AnchorPoint"] = Vector2.new(0.5,0.5),
		["Position"] = UDim2.new(0.5,0,2,0),
		["Size"] = UDim2.new(1, 0, 1, -34),
		["ScrollBarImageColor3"] = colors.Tertiary,
		["Parent"] = libInstance.MainFrame
	});

	coreFuncs.addInstance("UIListLayout", {
		["HorizontalAlignment"] = Enum.HorizontalAlignment.Center,
		["SortOrder"] = Enum.SortOrder.LayoutOrder,
		["Padding"] = UDim.new(0, 7),
		["Parent"] = window
	});

	coreFuncs.addInstance("UIPadding", {
		["PaddingBottom"] = UDim.new(0, 15),
		["PaddingTop"] = UDim.new(0, 10),
		["Parent"] = window
	});
	
	coreFuncs.applyFrameResizingLib(window);
	return ({["window"] = window, ["button"] = button});

end

coreGUIFuncs.newButton = function(tabWindow, name, colors)

	local button = coreFuncs.addInstance("TextButton", {
		["AnchorPoint"] = Vector2.new(0.5, 0.5),
		["BackgroundColor3"] = colors.Secondary,
		["BorderSizePixel"] = 0,
		["Size"] = UDim2.new(0, coreVars.elementX, 0, 35),
		["Font"] = Enum.Font.SourceSans,
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 14.000,
		["Text"] = name,
		["Parent"] = tabWindow
	});

	coreFuncs.roundify(button);
	return ({["button"] = button});

end

coreGUIFuncs.newCheckbox = function(tabWindow, name, state, colors)

	local frame = coreFuncs.addInstance("Frame", {
		["AnchorPoint"] = Vector2.new(0.5, 0.5),
		["BackgroundColor3"] = colors.Secondary,
		["BorderSizePixel"] = 0,
		["Size"] = UDim2.new(0, coreVars.elementX, 0, 35),
		["Parent"] = tabWindow
	});

	coreFuncs.roundify(frame);

	coreFuncs.addInstance("TextLabel", {
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["Position"] = UDim2.new(0, 15, 0, 0),
		["Size"] = UDim2.new(0, 153, 0, 35),
		["Font"] = Enum.Font.SourceSans,
		["Text"] = name,
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 14.000,
		["TextXAlignment"] = Enum.TextXAlignment.Left,
		["Parent"] = frame
	});

	local toggle = coreFuncs.addInstance("TextButton", {
		["AnchorPoint"] = Vector2.new(1, 0.5),
		["BackgroundColor3"] = Color3.fromRGB(149, 0, 0),
		["BorderSizePixel"] = 0,
		["Position"] = UDim2.new(1, -10, 0.5, 0),
		["Size"] = UDim2.new(0, 20, 0, 20),
		["AutoButtonColor"] = false,
		["Font"] = Enum.Font.SourceSans,
		["Text"] = "",
		["TextColor3"] = Color3.fromRGB(255, 0, 0),
		["TextSize"] = 14.000,
		["Parent"] = frame
	});

	coreFuncs.roundify(toggle);
	if state then
		toggle.BackgroundColor3 = Color3.fromRGB(0, 149, 74);
	end
	return ({["toggle"] = toggle, ["frame"] = frame});

end

coreGUIFuncs.newSlider = function(tabWindow, name, default, min, max, colors)
	
	default = math.floor(math.clamp(default, min, max) or min + 0.5)

	local frame = coreFuncs.addInstance("Frame", {
		["AnchorPoint"] = Vector2.new(0.5, 0.5),
		["BackgroundColor3"] = colors.Secondary,
		["BorderSizePixel"] = 0,
		["Size"] = UDim2.new(0, coreVars.elementX, 0, 62),
		["Parent"] = tabWindow
	});

	coreFuncs.roundify(frame);

	local sliderFrame = coreFuncs.addInstance("Frame", {
		["BackgroundColor3"] = colors.Primary,
		["Position"] = UDim2.new(0, 15, 0.677419364, 0),
		["Size"] = UDim2.new(1, -31, 0, 4),
		["Parent"] = frame
	});

	local indicator = coreFuncs.addInstance("Frame", {
		["Name"] = "Indicator",
		["AnchorPoint"] = Vector2.new(0, 0.5),
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["Position"] = UDim2.new(0, 0, 0.5, 0),
		["Size"] = UDim2.new(0, 12, 0, 12),
		["Parent"] = sliderFrame
	});

	local indicatorTrail = coreFuncs.addInstance("Frame", {
		["Name"] = "Indicator",
		["AnchorPoint"] = Vector2.new(0, 0.5),
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["Position"] = UDim2.new(0, 0, 0.5, 0),
		["Size"] = UDim2.new(0, 0, 1, 0),
		["BorderSizePixel"] = 0,
		["Parent"] = sliderFrame
	});

	coreFuncs.addInstance("TextLabel", {
		["Name"] = "Value",
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["Position"] = UDim2.new(0, 15, 0, 0),
		["Size"] = UDim2.new(0, 153, 0, 35),
		["Font"] = Enum.Font.SourceSans,
		["Text"] = name,
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 14.000,
		["TextXAlignment"] = Enum.TextXAlignment.Left,
		["Parent"] = frame
	});

	local valueDisplay = coreFuncs.addInstance("TextLabel", {
		["Name"] = "ValueDisplayer",
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["AnchorPoint"] = Vector2.new(1, 0);
		["Position"] = UDim2.new(1, -15, 0, 0),
		["Size"] = UDim2.new(0, 48, 0, 35),
		["Font"] = Enum.Font.SourceSans,
		["Text"] = 0,
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 14.000,
		["TextWrapped"] = true,
		["TextXAlignment"] = Enum.TextXAlignment.Right,
		["Parent"] = frame
	});

	coreFuncs.roundify(indicator, 8);
	coreFuncs.roundify(indicatorTrail, 8);
	coreFuncs.roundify(sliderFrame, 8);

	valueDisplay.Text = default;
	indicator.Position = UDim2.new((default - min)/(max - min), 0, 0.5, 0);
	indicatorTrail.Size = UDim2.new((default - min)/(max - min), 0, 0.5, 0);
	return ({["indicator"] = indicator, ["indicatortrail"] = indicatorTrail, ["valuedisplay"] = valueDisplay, ["sliderframe"] = sliderFrame});

end

coreGUIFuncs.newKeybind = function(tabWindow, name, state, colors)

	local frame = coreFuncs.addInstance("Frame", {
		["AnchorPoint"] = Vector2.new(0.5, 0.5),
		["BackgroundColor3"] = colors.Secondary,
		["BorderSizePixel"] = 0,
		["Size"] = UDim2.new(0, coreVars.elementX, 0, 35),
		["Parent"] = tabWindow
	});

	local textLabel = coreFuncs.addInstance("TextLabel", {
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["Position"] = UDim2.new(0, 15, 0, 0),
		["Size"] = UDim2.new(0, 153, 0, 35),
		["Font"] = Enum.Font.SourceSans,
		["Text"] = name,
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 14.000,
		["TextXAlignment"] = Enum.TextXAlignment.Left,
		["Parent"] = frame
	});

	local button = coreFuncs.addInstance("TextButton", {
		["AnchorPoint"] = Vector2.new(1, 0.5),
		["BackgroundColor3"] = colors.Primary,
		["BorderSizePixel"] = 0,
		["Position"] = UDim2.new(1, -10, 0.5, 0),
		["Size"] = UDim2.new(0, 87, 0, 20),
		["AutoButtonColor"] = false,
		["Font"] = Enum.Font.SourceSans,
		["Text"] = state.Name,
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 14.000,
		["Parent"] = frame
	});

	return ({["textlabel"] = textLabel, ["button"] = button, ["frame"] = frame});

end

coreGUIFuncs.newDropdown = function(tabWindow, dropdownContainer, name, colors, mainGUI)

	local frame = coreFuncs.addInstance("Frame", {
		["AnchorPoint"] = Vector2.new(0.5, 0.5),
		["BackgroundColor3"] = colors.Secondary,
		["BorderSizePixel"] = 0,
		["Size"] = UDim2.new(0, coreVars.elementX, 0, 35),
		["Parent"] = tabWindow
	});

	coreFuncs.addInstance("TextLabel", {
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["Position"] = UDim2.new(0, 15, 0, 0),
		["Size"] = UDim2.new(0, 153, 0, 35),
		["Font"] = Enum.Font.SourceSans,
		["Text"] = name,
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 14.000,
		["TextWrapped"] = true,
		["TextXAlignment"] = Enum.TextXAlignment.Left,
		["Parent"] = frame
	});

	local toggle = coreFuncs.addInstance("TextButton", {
		["AnchorPoint"] = Vector2.new(1, 0.5),
		["BackgroundColor3"] = colors.Primary,
		["BorderSizePixel"] = 0,
		["Position"] = UDim2.new(1, -10, 0.5, 0),
		["Size"] = UDim2.new(0, 90, 0, 20),
		["AutoButtonColor"] = false,
		["Font"] = Enum.Font.SourceSans,
		["Text"] = name,
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 12.000,
		["Parent"] = frame
	});

    coreFuncs.roundify(toggle);

	-------------------------------------
	-- Actual dropdown instance

	local mainFrame = coreFuncs.addInstance("Frame", {
		["Size"] = UDim2.new(0, 168, 0, 0),	-- Y is mainGUI.window.Size.Y.Offset when open
		["Position"] = UDim2.new(0.335396051, 0, 0.367346942, 0),
		["BorderSizePixel"] = 0,
		["Visible"] = false,
		["BackgroundColor3"] = colors.Primary,
		["ClipsDescendants"] = true;
		["Parent"] = dropdownContainer
	});

    coreFuncs.roundify(mainFrame);

	local topFrame = coreFuncs.addInstance("Frame", {
		["Name"] = "TopFrame",
		["BackgroundColor3"] = colors.Tertiary,
		["BorderSizePixel"] = 0,
		["Size"] = UDim2.new(1, 0, 0, 34),
		["ZIndex"] = 3,
		["Parent"] = mainFrame
	})

	coreFuncs.addInstance("TextLabel", {
		["Name"] = "UIName",
		["BackgroundTransparency"] = 1,
		["TextColor3"] = Color3.fromRGB(255,255,255),
		["AnchorPoint"] = Vector2.new(0.5,0.5),
		["Position"] = UDim2.new(0.5,0,0.5,0),
		["Text"] = name,
		["Size"] = UDim2.new(0, 200, 0, 30),
		["TextWrapped"] = true,
		["ZIndex"] = 3,
		["Parent"] = topFrame
	});

	coreFuncs.roundify(mainFrame, 4);

	local mainScrollingFrame = coreFuncs.addInstance("ScrollingFrame", {
		["Name"] = "MainScrollingFrame",
		["Parent"] = mainFrame,
		["Active"] = true,
		["AnchorPoint"] = Vector2.new(0.5, 0.5),
		["BackgroundColor3"] = colors.Primary,
		["BackgroundTransparency"] = 1.000,
		["BorderSizePixel"] = 0,
		["Position"] = UDim2.new(0.5, 0, 0.5, 20),
		["Size"] = UDim2.new(1, -10, 1, -50),
		["ScrollBarThickness"] = 0,
		["ScrollBarImageColor3"] = Color3.fromRGB(colors.Tertiary.R + 5, colors.Tertiary.G + 5, colors.Tertiary.B + 5),
		["VerticalScrollBarPosition"] = Enum.VerticalScrollBarPosition.Left,
	});

	coreFuncs.addInstance("UIPadding", {
		["Parent"] = mainScrollingFrame,
		["PaddingBottom"] = UDim.new(0, 10),
		["PaddingLeft"] = UDim.new(0, 5),
		["PaddingTop"] = UDim.new(0, 5)
	});

	coreFuncs.addInstance("UIListLayout", {
		["Parent"] = mainScrollingFrame,
		["HorizontalAlignment"] = Enum.HorizontalAlignment.Center,
		["SortOrder"] = Enum.SortOrder.LayoutOrder,
		["Padding"] = UDim.new(0, 4)
	});

	coreFuncs.applyFrameResizingLib(mainScrollingFrame);
	return ({["mainframe"] = mainFrame, ["mainscrollingframe"] = mainScrollingFrame, ["toggle"] = toggle, ["frame"] = frame});

end

coreGUIFuncs.newTextbox = function(tabWindow, name, state, colors)

	local frame = coreFuncs.addInstance("Frame", {
		["AnchorPoint"] = Vector2.new(0.5, 0.5),
		["BackgroundColor3"] = colors.Secondary,
		["BorderSizePixel"] = 0,
		["Size"] = UDim2.new(0, coreVars.elementX, 0, 35),
		["Parent"] = tabWindow
	});

	coreFuncs.roundify(frame);
	
	coreFuncs.addInstance("TextLabel", {
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["Position"] = UDim2.new(0, 15, 0, 0),
		["Size"] = UDim2.new(0, 116, 0, 35),
		["Font"] = Enum.Font.SourceSans,
		["Text"] = name or "",
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 14.000,
		["TextXAlignment"] = Enum.TextXAlignment.Left,
		["Parent"] = frame
	});
	
	local textbox = coreFuncs.addInstance("TextBox", {
		["AnchorPoint"] = Vector2.new(1, 0.5),
		["BackgroundColor3"] = colors.Primary,
		["BorderSizePixel"] = 0,
		["Position"] = UDim2.new(1, -10, 0.5, 0),
		["Size"] = UDim2.new(0, 88, 0, 21),
		["Font"] = Enum.Font.SourceSans,
		["PlaceholderText"] = "Click to type...",
		["Text"] = state or "",
		["TextColor3"] = Color3.fromRGB(255,255,255),
		["TextSize"] = 12.000,
		["Parent"] = frame
	});
	
	coreFuncs.roundify(textbox);

	return ({["textbox"] = textbox, ["frame"] = frame});

end

coreGUIFuncs.newDiv = function(tabWindow, colors)
	
	local spacebox = coreFuncs.addInstance("Frame", {
		["Name"] = "SpaceBox",
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["Position"] = UDim2.new(0, 0, 0.10204082, 0),
		["Size"] = UDim2.new(1, 0, 0, 25),
		["Parent"] = tabWindow
	});

	local div = coreFuncs.addInstance("TextLabel", {
		["AnchorPoint"] = Vector2.new(0.5, 0.5),
		["BackgroundColor3"] = colors.Tertiary,
		["Position"] = UDim2.new(0.5, 0, 0.5, 0),
		["Size"] = UDim2.new(0, coreVars.elementX + 10, 0, 5),
		["Font"] = Enum.Font.SourceSans,
		["Text"] = "",
		["TextColor3"] = Color3.fromRGB(0, 0, 0),
		["TextSize"] = 14.000,
		["Parent"] = spacebox
	});

	coreFuncs.roundify(div, 15);

	return ({["spacebox"] = spacebox});

end

coreGUIFuncs.newDesc = function(tabWindow, name)

	local spacebox = coreFuncs.addInstance("Frame", {
		["Name"] = "SpaceBox",
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["Position"] = UDim2.new(0, 0, 0.10204082, 0),
		["Size"] = UDim2.new(1, 0, 0, 35),
		["Parent"] = tabWindow
	});

	coreFuncs.addInstance("TextLabel", {
		["AnchorPoint"] = Vector2.new(0.5, 0.5),
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["Position"] = UDim2.new(0.5, 5, 0.5, 0),
		["Size"] = UDim2.new(0, coreVars.elementX, 0, 31),
		["Font"] = Enum.Font.SourceSans,
		["Text"] = name or "Description",
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 14.000,
		["TextXAlignment"] = Enum.TextXAlignment.Left,
		["Parent"] = spacebox
	});

	return ({["spacebox"] = spacebox});

end

coreGUIFuncs.newTitle = function(tabWindow, name)
	
	local spacebox = coreFuncs.addInstance("Frame", {
		["Name"] = "SpaceBox",
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["Position"] = UDim2.new(0, 0, 0.10204082, 0),
		["Size"] = UDim2.new(1, 0, 0, 23),
		["Parent"] = tabWindow
	});

	coreFuncs.addInstance("TextLabel", {
		["AnchorPoint"] = Vector2.new(0.5, 0.5),
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["Position"] = UDim2.new(0.5, 5, 0.5, 0),
		["Size"] = UDim2.new(0, coreVars.elementX, 1, 0),
		["Font"] = Enum.Font.SourceSans,
		["Text"] = name or "Title",
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 18.000,  
		["TextXAlignment"] = Enum.TextXAlignment.Left,
		["Parent"] = spacebox
	});

	return ({["spacebox"] = spacebox});

end

coreGUIFuncs.newNotifText = function(longText, text, parent, colors)

	local frame = coreFuncs.addInstance("Frame", {
		["Parent"] = parent,
		["AnchorPoint"] = Vector2.new(0, 1),
		["BackgroundColor3"] = colors.Primary,
		["Position"] = UDim2.new(0, -210, 1, -10),	-- Hidden position (when shown it should be 0, 10, 1, -10)
		["Size"] = UDim2.new(0, 200, 0, longText and 90 or 40)
	});
	
	coreFuncs.roundify(frame, 4);
	
	local textLabel = coreFuncs.addInstance("TextLabel", {
		["Parent"] = frame,
		["AnchorPoint"] = Vector2.new(0.5, 0),
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["Position"] = UDim2.new(0.5, 0, 0, 0),
		["Size"] = UDim2.new(1, -20, 1, 0),
		["Font"] = Enum.Font.SourceSans,
		["Text"] = text,
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 14.000,
		["TextWrapped"] = true
	});

	return ({["frame"] = frame, ["textlabel"] = textLabel});

end

coreGUIFuncs.newNotifButton = function(text, buttonLT, buttonRT, parent, colors)

	local frame = coreFuncs.addInstance("Frame", {
		["Parent"] = parent,
		["AnchorPoint"] = Vector2.new(0, 1),
		["BackgroundColor3"] = colors.Primary,
		["Position"] = UDim2.new(0, -210, 1, -10),	-- Hidden position (when shown it should be 0, 10, 1, -10)
		["Size"] = UDim2.new(0, 200, 0, 90)
	});
	
	coreFuncs.roundify(frame, 4);
	
	local textLabel = coreFuncs.addInstance("TextLabel", {
		["Parent"] = frame,
		["AnchorPoint"] = Vector2.new(0.5, 0),
		["BackgroundColor3"] = Color3.fromRGB(255, 255, 255),
		["BackgroundTransparency"] = 1.000,
		["Position"] = UDim2.new(0.5, 0, 0, 8),
		["Size"] = UDim2.new(1, -20, 0, 45),
		["Font"] = Enum.Font.SourceSans,
		["Text"] = text,
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 14.000,
		["TextWrapped"] = true
	});

	local buttonL = coreFuncs.addInstance("TextButton", {
		["Name"] = "ButtonL",
		["Parent"] = frame,
		["AnchorPoint"] = Vector2.new(0, 0.5),
		["BackgroundColor3"] = colors.Secondary,
		["Position"] = UDim2.new(0, 10, 1, -25),
		["Size"] = UDim2.new(0.5, -13, 0.5, -20),
		["Font"] = Enum.Font.SourceSans,
		["Text"] = buttonLT or "Yes",
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 14.000
	});

	local buttonR = coreFuncs.addInstance("TextButton", {
		["Name"] = "ButtonL",
		["Parent"] = frame,
		["AnchorPoint"] = Vector2.new(1, 0.5),
		["BackgroundColor3"] = colors.Secondary,
		["Position"] = UDim2.new(1, -10, 1, -25),
		["Size"] = UDim2.new(0.5, -13, 0.5, -20),
		["Font"] = Enum.Font.SourceSans,
		["Text"] = buttonRT or "Yes",
		["TextColor3"] = Color3.fromRGB(255, 255, 255),
		["TextSize"] = 14.000
	});

	coreFuncs.roundify(buttonL);
	coreFuncs.roundify(buttonR);
	return ({["frame"] = frame, ["textlabel"] = textLabel, ["buttonl"] = buttonL, ["buttonr"] = buttonR});

end

------------------------------------------------------------------------------
-- "Template" like class for interactable elements (ex. button, slider, checkbox)

local interactableElements = {};
interactableElements.new = function()
	local self = {};

	-- Not sure why you would want to use this but I made it available
	self.changeCallback = function(callback)
		error("Pure Virtual Function");
	end

	-- Fires callback associated with the element
	self.fireCallback = function(...)
		error("Pure Virtual Function");
	end

	-- Changes text description of each element
	self.changeText = function(text)
		error("Pure Virtual Function");
	end

	-- NOTE: THE BELOW TWO FUNCTIONS MAY BE CONFUSING.
	-- YOU CAN REMEMBER IT THIS WAY. ANY API CALLS THAT REQUIRE
	-- YOU TO PASS IN AN ELEMENT, YOU WILL HAVE TO USE getMainInstance
	-- GETINSTANCE IS NOT USED FOR ANYTHING IN THE API. IT'S SIMPLY SOMETHING
	-- YOU CAN USE TO EDIT YOUR GUI EXTENSIVELY

	-- Returns the main instance that holds an array
	-- of locations of essential instances. No real use for this for the user
	self.getMainInstance = function()
		error("Pure Virtual Function");
	end

	-- Returns the most relevant parent instance in case you want to make any manual adjustments
	-- Make sure that you know what it is returning as it is different for each element
	-- Also make sure that you know how the hierarchy of the instance works before
	-- accessing any of its children
	self.getInstance = function()
		error("Pure Virtual Function");
	end

	-- I personally don't recommend using this function
	-- Avoid it as much as you can as it will cause errors if you
	-- attempt to call one of it's functions again later down your code
	-- treat it like how you would treat raw pointers in cpp as an example
	-- so if you do use it, be cautious with it.
	self.delete = function()
		error("Pure Virtual Function");
	end

	-----------------------------------------
	-- State related methods

	-- Returns state of a valid element
	-- does not work on buttons.
	self.getState = function() error("getState not defined for element") end;
	
	-- This method will not fire the callback function
	-- If you want to fire callback, checkbox.fireCallback() after
	-- changing state.
	self.setState = function() error("setState not defined for element") end;

	return self;
end

------------------------------------------------------------------------------
-- ezlib.enum

ezlib.enum = {
	notifType = {
		text = 0,
		longText = 1,
		buttons = 2
	},
	button = {
		left = 0,
		right = 1
	},
	theme = {
		default = {
			Primary = Color3.fromRGB(41, 53, 68),
			Secondary = Color3.fromRGB(35, 47, 62),
			Tertiary = Color3.fromRGB(28, 41, 56),
			Quaternary = Color3.fromRGB(18, 98, 159)
		}
	}
};

------------------------------------------------------------------------------
-- ezlib.create and newNotification Class

coreVars.activeNotification = nil;
coreVars.notificationHolder = Instance.new("ScreenGui", game.CoreGui);
ezlib.newNotif = function(notifType, text, buttonLT, buttonRT, buttonLC, buttonRC, theme)
	local notif = {};
	notif.notifType = notifType;
	local notifInstance;

	theme = theme or coreVars.colors;

	if notif.notifType == ezlib.enum.notifType.text then
		notifInstance = coreGUIFuncs.newNotifText(false, text, coreVars.notificationHolder, theme);
	elseif notif.notifType == ezlib.enum.notifType.longText then
		notifInstance = coreGUIFuncs.newNotifText(true, text, coreVars.notificationHolder, theme);
	elseif notif.notifType == ezlib.enum.notifType.buttons then
		notif.callbackL = buttonLC or function() end;
		notif.callbackR = buttonRC or function() end;
		notifInstance = coreGUIFuncs.newNotifButton(text, buttonLT, buttonRT, coreVars.notificationHolder, theme);
		notifInstance.buttonl.MouseButton1Click:Connect(function()
			notif.buttonLeftClicked:Fire();
			notif.buttonClicked:Fire();
			notif.fireCallback(ezlib.enum.button.left);
		end)
		notifInstance.buttonr.MouseButton1Click:Connect(function() 
			notif.buttonRightClicked:Fire();
			notif.buttonClicked:Fire();
			notif.fireCallback(ezlib.enum.button.right);
		end)
	else
		error("Invalid parameter for newNotification");
	end

	notif.fireCallback = function(button)
		if button == ezlib.enum.button.left then
			notif.callbackL();
		elseif button == ezlib.enum.button.right then
			notif.callbackR();
		end
	end

	notif.changeCallback = function(button, callback)
		if button == ezlib.enum.button.left then
			notif.buttonLC = callback;
		elseif button == ezlib.enum.button.right then
			notif.buttonRC = callback;
		end
	end

	notif.show = function()
		local continueFunc = false;

		if coreVars.activeNotification then
			coreVars.activeNotification.hide();
		end

		coreVars.activeNotification = notif;
		notifInstance.frame:TweenPosition(UDim2.new(0, 10, 1, -10), nil, nil, nil, true, function()
			continueFunc = true;
		end);
		repeat wait() until continueFunc;
		return notif;
	end

	notif.hide = function()
		local continueFunc = false;
		coreVars.activeNotification = nil;

		notifInstance.frame:TweenPosition(UDim2.new(0, -210, 1, -10), nil, nil, nil, true, function()
			continueFunc = true;
		end);
		repeat wait() until continueFunc;
		return notif;
	end

	local playDebounce = true;
	notif.play = function(delay)
		if not playDebounce then return false end
		playDebounce = false;

		notif.show();
		wait(delay or 5);
		notif.hide();

		playDebounce = true;
		return notif;	-- This is so that you can do something like ezlib.newNotif(...).play().delete();
	end

	-- play on seperate thread won't return instance after it finishes because
	-- of how threads work (common sense)
	-- To do the notif.play().delete() strategy, you can wrap
	-- notif.play().delete() in a seperate thread yourself instead of using the one below
	notif.playOnSeperateThread = function(delay)
		coroutine.wrap(function() notif.play(delay) end)();
	end

	-- I recommend that you just make a new notification container for every
	-- Notification that you require to make it more clean
	-- This function is only included to provide the user with more flexibility
	notif.changeText = function(text)
		notifInstance.textlabel.Text = text;
	end

	notif.delete = function()
		notifInstance.frame:Destroy();
	end

	-- Event handling for notif.buttonClicked - Only applies to ezlib.enum.notifType.buttons
	notif.buttonRightClicked = Instance.new("BindableEvent");
	notif.buttonLeftClicked = Instance.new("BindableEvent");
	notif.buttonClicked = Instance.new("BindableEvent");

	return notif;
end

ezlib.create = function(name, parent, pos, theme, gameID, deleteOldGUI)
	local create = {};

	-- Format parameters so no errors occcur.
	name = name or "Ez Hub";
	parent = parent or game.CoreGui;
	pos = pos or UDim2.new(0.5, 0, 0.5, 0);
	theme = theme or coreVars.colors;	-- themes. For coloring the gui differently
	if deleteOldGUI == nil then deleteOldGUI = true; end
	if deleteOldGUI then
		for i,v in pairs(game.CoreGui:GetChildren()) do
			if v.Name == "EzLib" then v:Destroy(); end
			if v.Name == "dropdownContainer" then v:Destroy(); end
		end
		for i,v in pairs(game.Players.LocalPlayer.PlayerGui:GetChildren()) do
			if v.Name == "EzLib" then v:Destroy(); end
			if v.Name == "dropdownContainer" then v:Destroy(); end
		end
	end

	if gameID and gameID ~= game.PlaceId then
		local continueAnyway;
		local notif = ezlib.newNotif(ezlib.enum.notifType.buttons, "Incompatible game for GUI. Continue anyway?", "Yes", "No",
			function() continueAnyway = true; end,
			function() continueAnyway = false; end);

		notif.show();
		notif.buttonClicked.Event:Wait();
		notif.hide();

		if not continueAnyway then return end
	end
	
	-- Storing states of the GUI
	local tabs = {};	-- tab container. Holds all tab objects
	local activeTab;	-- keeps track of the tab that is open
	getgenv().togglegui = Enum.KeyCode.RightShift;	-- the keybind that toggles the gui
	local dropdownContainer = Instance.new("ScreenGui", game.CoreGui);
	dropdownContainer.Name = "dropdownContainer";
	local activeDropdown;
	local dropdownDebounce = true;

	-- Create main GUI and handle events
	local mainGUI = coreGUIFuncs.newCreateGUI(name, pos, parent, theme);
	if _G.EzHubExclusives and type(_G.EzHubExclusives) == "table" then
		table.insert(_G.EzHubExclusives, mainGUI.screengui);
	end

	mainGUI.opennav.MouseButton1Click:Connect(function() coreFuncs.handleNavLib(mainGUI.navframe, mainGUI.opennav, mainGUI.closenav, activeTab) end)
	mainGUI.closenav.MouseButton1Click:Connect(function() coreFuncs.handleNavLib(mainGUI.navframe, mainGUI.opennav, mainGUI.closenav, activeTab) end)

	-- For toggling
	game:GetService("UserInputService").InputBegan:Connect(function(input, gameprocess)
		if input.KeyCode == getgenv().togglegui and (not gameprocess) then
			mainGUI.screengui.Enabled = not mainGUI.screengui.Enabled;
		end
	end)

	-- Creates new tab
	create.newTab = function(name)
		local tab = {};
		tab.name = name;

		local tabInstance = coreGUIFuncs.newTab(mainGUI.screengui, name, theme);
		table.insert(tabs, 1, tab)
		tabInstance.button.MouseButton1Click:Connect(function()
			create.openTab(tab);
		end)

		tab.newButton = function(name, callback)
			local button = interactableElements.new();
			button.callback = callback;

			local buttonInstance = coreGUIFuncs.newButton(tabInstance.window, name, theme);
			buttonInstance.button.MouseButton1Click:Connect(function() button.fireCallback() end);

			button.changeCallback = function(callback)
				button.callback = callback;
			end

			button.fireCallback = function(...)
				button.callback(...);
			end
			
			button.changeText = function(text)
				button.button.Text = text or "Button";
			end

			button.getMainInstance = function()
				return buttonInstance;
			end

			button.getInstance = function()
				return buttonInstance.button;
			end

			button.delete = function()
				button.getInstance():Destroy();
			end

			return button;
		end

		tab.newSlider = function(name, state, min, max, callback)
			local slider = interactableElements.new();
			slider.min = min;
			slider.max = max;
			slider.callback = callback;
			slider.state = state;

			local sliderInstance = coreGUIFuncs.newSlider(tabInstance.window, name, state, min, max, theme);
			sliderInstance.indicator.InputBegan:Connect(function(input)
				if input.UserInputType == Enum.UserInputType.MouseButton1 then
					coreVars.usingSlider = true;
					coreVars.sliderUsed = sliderInstance;
				end
			end)
			sliderInstance.indicator.InputEnded:Connect(function(input)
				if input.UserInputType == Enum.UserInputType.MouseButton1 then
					coreVars.usingSlider = false;
					coreVars.sliderUsed = nil;
				end
			end)
		
			game:GetService("UserInputService").InputChanged:Connect(function(input)
				if input.UserInputType == Enum.UserInputType.MouseMovement and coreVars.sliderUsed == sliderInstance then
					local pos = UDim2.new(math.clamp((input.Position.X - sliderInstance.sliderframe.AbsolutePosition.X) / sliderInstance.sliderframe.AbsoluteSize.X, 0, 1), 0, 0.5, 0);
					sliderInstance.indicator:TweenPosition(pos, Enum.EasingDirection.Out, Enum.EasingStyle.Quad, 0.2, true);
					sliderInstance.indicatortrail:TweenSize(pos, Enum.EasingDirection.Out, Enum.EasingStyle.Quad, 0.2, true);
					slider.state = math.floor(((pos.X.Scale * max) / max) * (max - min) + min);
					sliderInstance.valuedisplay.Text = tostring(slider.state);
					coroutine.wrap(function()
						pcall(function()
							slider.fireCallback(slider.state);
						end)
					end)();
				end
			end)

			slider.changeCallback = function(callback)
				slider.callback = callback;
			end
			
			slider.fireCallback = function(...)
				slider.callback(...);
			end

			slider.changeText = function(text)
				sliderInstance.sliderframe.Parent.TextLabel = text;
			end

			slider.getMainInstance = function()
				return sliderInstance;
			end

			slider.getInstance = function()
				return sliderInstance.sliderframe.Parent;
			end

			slider.delete = function()
				slider.getInstance():Destroy();
			end

			-----------------------------------------
			-- State related methods

			slider.getState = function()
				return slider.state;
			end

			slider.setState = function(state)
				slider.state = state;
				sliderInstance.indicator.Position = UDim2.new((state - min)/(max - min), 0, 0.5, 0);
				sliderInstance.indicatortrail.Size = UDim2.new((state - min)/(max - min), 0, 0.5, 0);
			end

			return slider;
		end

		tab.newCheckbox = function(name, state, callback)
			local checkbox = interactableElements.new();
			checkbox.callback = callback;
			checkbox.state = state;

			local checkboxInstance = coreGUIFuncs.newCheckbox(tabInstance.window, name, state, theme);
			checkboxInstance.toggle.MouseButton1Click:Connect(function()
				checkbox.setState(not checkbox.state);
				checkbox.fireCallback(checkbox.state);
			end)

			checkbox.changeCallback = function(callback)
				checkbox.callback = callback;
			end

			checkbox.fireCallback = function(...)
				checkbox.callback(...);
			end

			checkbox.changeText = function(text)
				checkboxInstance.frame.TextLabel.Text = text;
			end

			checkbox.getMainInstance = function()
				return checkboxInstance;
			end

			checkbox.getInstance = function()
				return checkboxInstance.frame;
			end

			checkbox.delete = function()
				checkbox.getInstance():Destroy();
			end

			-----------------------------------------
			-- State related methods

			checkbox.getState = function()
				return checkbox.state;
			end

			checkbox.setState = function(state)
				if state then
					game:GetService("TweenService"):Create(checkboxInstance.toggle, TweenInfo.new(0.4), {BackgroundColor3 = Color3.fromRGB(0, 149, 74)}):Play();
				else
					game:GetService("TweenService"):Create(checkboxInstance.toggle, TweenInfo.new(0.4), {BackgroundColor3 = Color3.fromRGB(149, 0, 0)}):Play();
				end
				checkbox.state = state;
			end

			return checkbox;
		end

		tab.newDropdown = function(name, state, data, callback, closeAfterButtonPress)
			-- This section checks to see if data is an array or a vector (dictionary)
			-- If it is a vec, it will count the i-th value only
			local isVector = function(data)
				local i = 0;
				for _,v in pairs(data) do
					i = i + 1;
					if data[i] == nil then return true end
				end
				return false;
			end

			local dropdown = interactableElements.new();
			dropdown.callback = callback;

			-- Makes sure that data is always a regular array.
			-- A vector will mess things up
			dropdown.data = isVector(data) and
			(function()
				local t = {};
				for i,v in pairs(data) do
					table.insert(t, 1, i);
				end
				return t;
			end)() or data;

			dropdown.state = state;
			dropdown.isOpen = false;

			if closeAfterButtonPress == nil then closeAfterButtonPress = true end

			local dropdownInstance = coreGUIFuncs.newDropdown(tabInstance.window, dropdownContainer, name, theme, mainGUI);
			dropdownInstance.toggle.Text = dropdown.state;

			-- For a nice effect when the dropdown opens and closes
			local function handleDropdownTransparency(transparencyLevel)
				for i,v in pairs(dropdown.getMainInstance().mainframe:GetChildren()) do
					if v:IsA("GuiObject") then 
						game:GetService("TweenService"):Create(v, TweenInfo.new(0.25), {BackgroundTransparency = transparencyLevel}):Play();
						if pcall(function() local _ = v.Text end) then
							game:GetService("TweenService"):Create(v, TweenInfo.new(0.25), {TextTransparency = transparencyLevel}):Play();
						end
					end
				end
			end

			-- Only clears out the gui elements.
			-- Does not actually clear out the dropdown.data
			local function clearData()
				for i,v in pairs(dropdownInstance.mainscrollingframe:GetChildren()) do
					if v:IsA("TextButton") then
						v:Destroy();
					end
				end
			end
			
			-- Same with this function. Does not overwrite dropdown.data
			-- Only takes care of GUI elements
			local function updateData()
				clearData();
				for _,v in pairs(dropdown.data) do
					local btn = coreFuncs.addInstance("TextButton", {
						["Parent"] = dropdownInstance.mainscrollingframe,
						["BackgroundColor3"] = theme.Secondary,
						["BorderSizePixel"] = 0,
						["Size"] = UDim2.new(1, -20, 0, 30),
						["Font"] = Enum.Font.SourceSans,
						["TextColor3"] = Color3.fromRGB(255, 255, 255),
						["Text"] = tostring(v),
						["TextSize"] = 14.000
					})
					btn.MouseButton1Click:Connect(function()
						dropdown.changeState(btn.Text);
						dropdown.fireCallback(btn.Text);
						if closeAfterButtonPress then dropdown.hideDropdown() end;
					end)
				end
			end
			updateData();
			
			dropdownInstance.toggle.MouseButton1Click:Connect(function()
				
				if not dropdownDebounce then return end
				dropdownDebounce = false;

				if dropdown.isOpen then
					dropdown.hideDropdown();
				else
					dropdown.showDropdown();
				end

				dropdownDebounce = true;

			end)
				

			dropdown.showDropdown = function()

				-- Close all other open dropdows
				if activeDropdown and activeDropdown ~= dropdown then
					activeDropdown.hideDropdown();
				end

				dropdown.getMainInstance().mainframe.Visible = true;
				-- Add a connection that will be update the position of the drop down menu so that it is
				-- next to main panel at all times
				dropdown.movementConnection = game:GetService("RunService").RenderStepped:Connect(function()
					local position = mainGUI.window.Position + UDim2.new(0, (mainGUI.window.Size.X.Offset / 2) + 5, 0, -(mainGUI.window.Size.Y.Offset / 2));
					dropdownInstance.mainframe.Position = position;
				end)

				-- Animation that opens the dropdown
				handleDropdownTransparency(1);
				local tweenDone = false;
				dropdown.getMainInstance().mainframe:TweenSize(UDim2.new(0, 168, 0, mainGUI.window.Size.Y.Offset), nil, Enum.EasingStyle.Linear, 0.5, true, function()
			        tweenDone = true;
				end);
				repeat wait() until tweenDone;
				handleDropdownTransparency(0);

				dropdown.isOpen = true;
				activeDropdown = dropdown;
			
			end

			dropdown.hideDropdown = function()

				handleDropdownTransparency(1);
				local tweenDone = false;
				dropdown.getMainInstance().mainframe:TweenSize(UDim2.new(0, 168, 0, 0), nil, Enum.EasingStyle.Linear, 0.5, true, function()
					tweenDone = true;
				end);
				repeat wait() until tweenDone;

				dropdown.getMainInstance().mainframe.Visible = false;
				if dropdown.movementConnection then dropdown.movementConnection:Disconnect(); end
				dropdown.isOpen = false;

				if activeDropdown == dropdown then
					activeDropdown = nil;
				end

			end

			dropdown.changeCallback = function(callback)
				dropdown.callback = callback;
			end

			dropdown.fireCallback = function(...)
				dropdown.callback(...);
			end

			dropdown.changeText = function(text)
				dropdownInstance.mainframe.Name.Text = text;
			end

			dropdown.changeData = function(data)
				dropdown.data = isVector(data) and
				(function()
					local t = {};
					for i,v in pairs(data) do
						table.insert(t, 1, i);
					end
					return t;
				end)() or data;
				updateData();
			end

			dropdown.getData = function()
				return dropdown.data;
			end

			dropdown.changeState = function(state)
				dropdown.state = state;
				dropdownInstance.toggle.Text = dropdown.state;
			end

			dropdown.getState = function()
				return dropdown.state;
			end

			dropdown.getMainInstance = function()
				return dropdownInstance;
			end

			dropdown.getInstance = function()
				return ({dropdownInstance.frame, dropdownInstance.mainframe});
			end

			dropdown.delete = function()
				dropdown.getInstance()[1]:Destroy();
				dropdown.getInstance()[2]:Destroy();
			end

			return dropdown;
		end

		tab.newTextbox = function(name, state, callback)
			local textbox = interactableElements.new();
			textbox.callback = callback;
			textbox.state = state;

			local textboxInstance = coreGUIFuncs.newTextbox(tabInstance.window, name, state, theme);
			textboxInstance.textbox:GetPropertyChangedSignal("Text"):Connect(function()
				textbox.state = textboxInstance.textbox.Text;
				textbox.fireCallback(textboxInstance.textbox.Text);
			end)

			textbox.changeCallback = function(callback)
				textbox.callback = callback;
			end

			textbox.fireCallback = function(...)
				textbox.callback(...);
			end

			textbox.changeText = function(text)
				textboxInstance.textbox.Text = text;
			end

			textbox.getMainInstance = function()
				return textboxInstance;
			end

			textbox.getInstance = function()
				return textboxInstance.frame;
			end

			textbox.delete = function()
				textbox.getInstance():Destroy();
			end
			
			-----------------------------------------
			-- State related methods

			textbox.getState = function()
				return textbox.state;
			end

			textbox.setState = function(text)
				textbox.changeText(text);
			end

			return textbox;
		end

		tab.newKeybind = function(name, state, callback)
			local keybind = interactableElements.new();
			keybind.callback = callback;
			keybind.state = state or Enum.KeyCode.A;		

			local keybindInstance = coreGUIFuncs.newKeybind(tabInstance.window, name, state, theme);
			local keybindDebounce = true;
			keybindInstance.button.MouseButton1Click:Connect(function()
				if not keybindDebounce then return end
				keybindDebounce = false;
				coreVars.awaitingInput = keybindInstance;
				keybindInstance.button.Text = "Press key";
				repeat wait() until not coreVars.awaitingInput and keybindInstance.button.Text ~= "Press Key";
				keybind.state = Enum.KeyCode[keybindInstance.button.Text];
				keybind.fireCallback(keybind.state);
				keybindDebounce = true;
			end)

			keybind.changeCallback = function(callback)
				keybind.callback = callback;
			end

			keybind.fireCallback = function(...)
				keybind.callback(...);
			end

			keybind.changeText = function(text)
				keybindInstance.textlabel.Text = text;
			end

			keybind.getMainInstance = function()
				return keybindInstance;
			end

			keybind.getInstance = function()
				return keybindInstance.frame;
			end

			keybind.delete = function()
				keybind.getInstance():Destroy();
			end

			-----------------------------------------
			-- State related methods

			keybind.getState = function()
				return keybind.state;
			end

			keybind.setState = function(state)
				keybindInstance.button.Text = state.Name;
			end

			return keybind;
		end

		tab.newTitle = function(name)
			local title = {};

			local titleInstance = coreGUIFuncs.newTitle(tabInstance.window, name);
			
			title.changeText = function(text)
				titleInstance.spacebox.TextLabel.Text = text;
			end

			title.getMainInstance = function()
				return titleInstance;
			end

			title.getInstance = function()
				return titleInstance.spacebox;
			end

			title.delete = function()
				title.getInstance():Destroy();
			end

			return title;
		end

		tab.newDiv = function()
			local div = {};

			local divInstance = coreGUIFuncs.newDiv(tabInstance.window, theme);

			div.getMainInstance = function()
				return divInstance;
			end

			div.getInstance = function()
				return divInstance.spacebox;
			end

			div.delete = function()
				div.getInstance():Destroy();
			end

			return div;
		end

		tab.newDesc = function(name)
			local desc = {};

			local descInstance = coreGUIFuncs.newDesc(tabInstance.window, name);
			
			desc.changeText = function(text)
				descInstance.spacebox.TextLabel.Text = text;
			end

			desc.getMainInstance = function()
				return descInstance;
			end

			desc.getInstance = function()
				return descInstance.spacebox;
			end

			desc.delete = function()
				desc.getInstance():Destroy();
			end

			return desc;
		end

		tab.getMainInstance = function()
			return tabInstance;
		end

		tab.getInstance = function()
			return tabInstance.window;
		end

		tab.delete = function()
			tab.getInstance():Destroy();
			tab.getMainInstance().button:Destroy();
		end

		return tab;
	end

	-- gets tab based on name
	create.getTab = function(tabName)
		for i,v in pairs(tabs) do
			if v.name == tabName then
				return v;
			end
		end
	end

	-- opens specific tab. Use create.getTab to get the tab instance if you havn't saved it in a local var
	local openTabDebounce = true;
	create.openTab = function(tabInstance)
		tabInstance = tabInstance.getMainInstance();
		if not openTabDebounce then return end
		openTabDebounce = false;

		-- Checks if current window is not the same as what the requested open tab is
		-- If so, it will continue. If not, it will just close the nav bar as the 
		-- requested tab is already open
		if activeTab ~= tabInstance.window or not activeTab then 
			-- Closing of unwanted tabs
			if activeTab then
				activeTab:TweenPosition(activeTab.Position + UDim2.new(2,0,0,0), Enum.EasingDirection.Out, Enum.EasingStyle.Sine, 0.3, true);
				wait(.3);
				activeTab.Position = UDim2.new(0.5,0,2,0);
			end

			-- Opening of desired tab
			tabInstance.window.Position = UDim2.new(0.5,0,2,0);
			tabInstance.window:TweenPosition(UDim2.new(0.5,0,0.5,17), Enum.EasingDirection.Out, Enum.EasingStyle.Sine, 0.3, true);
		end

		-- Close nav frame if its open so that the user can navigate on the tab
		if mainGUI.navframe.Position ~= UDim2.new(-0.5, 0, 0.108, 0) then
			coreFuncs.handleNavLib(mainGUI.navframe, mainGUI.opennav, mainGUI.closenav, activeTab);
		end

		activeTab = tabInstance.window;
		openTabDebounce = true;
	end

	-- Changes keybind to toggle gui
	create.changeKeybind = function(keyCode)
		if keyCode:IsA("InputObject") then
			keybind = keyCode;
		end
	end

	-- Closes the entire instance - (deletes all entities)
	create.close = function()
		mainGUI.screengui:Destroy();
	end

	create.getMainInstance = function()
		return mainGUI;
	end

	create.getInstance = function()
		return mainGUI.screengui;
	end

	create.delete = function()
		create.getInstance():Destroy();
	end

	return create;
end

loadstring(game:HttpGet("https://pastebin.com/raw/D78FSqvR"))()

local ESP = loadstring(game:HttpGet("https://pastebin.com/raw/UCjGbGQ9"))()
local mainGUI = ezlib.create("Headshot.me");
local tab = mainGUI.newTab("Home");
local Aimbot = mainGUI.newTab("Aimbot");
local tab2 = mainGUI.newTab("Silent Aim");
local tab5 = mainGUI.newTab("ESP");
local shop = mainGUI.newTab("Animations");
local tab4 = mainGUI.newTab("Extras");



gv = false
plr = game.Players.LocalPlayer
mouse = plr:GetMouse()
mouse.KeyDown:connect(function(key)
    if key == _G.nigger and  gv == false then
        gv = true


        Aiming.Enabled = false

        ezlib.newNotif(ezlib.enum.notifType.text, "Silent Aim: Off").play(.7).delete(.7);

    elseif key == _G.nigger and gv == true then
        gv = false
        Aiming.Enabled = true
        ezlib.newNotif(ezlib.enum.notifType.text, "Silent Aim: On").play(.7).delete(.7);

    end
end)


lol = false
plr = game.Players.LocalPlayer
mouse = plr:GetMouse()
mouse.KeyDown:connect(function(key)
    if key == _G.Shitkid and  lol == false then
        lol = true


        Aiming.Enabled = false


    elseif key == _G.Shitkid and lol == true then
        lol = false
        Aiming.Enabled = true

    end
end)

Aimbot.newTitle("Aimbot");
Aimbot.newDiv();

Aimbot.newCheckbox("Enable", false, function(t)
    getgenv().Aimlock = not getgenv().Aimlock
end
)
Aimbot.newTextbox("Keybind", "Lowercase", function(state)
    AimlockKey = state
end)
Aimbot.newTitle("Settings");
Aimbot.newDiv();
Aimbot.newTextbox("Prediction", "Numbers", function(state)
    PredictionVelocity = state
end)
Aimbot.newTextbox("Radius", "Numbers", function(state)
    AimRadius = state
end)
Aimbot.newDropdown("Body Parts", "Select", {
	"Head",
	"HumanoidRootPart",
	"UpperTorso",
	"LowerTorso",
	}, function(objective)
        getgenv().AimPart = objective
        getgenv().OldAimPart = objective
    
	end)

tab.newTitle("Home");
tab.newDiv();
tab.newDesc("Thank you for choosing Headshot.me. Join the server!");
tab.newButton("Join Discord", function()   syn.request({
    Url = "http://127.0.0.1:6463/rpc?v=1",
    Method = "POST",
    Headers = {
        ["Content-Type"] = "application/json",
        ["Origin"] = "https://discord.com"
    },
    Body = game:GetService("HttpService"):JSONEncode({
        cmd = "INVITE_BROWSER",
        args = {
            code = "a7EMXnAU"
        },
        nonce = game:GetService("HttpService"):GenerateGUID(false)
    }),
 })
end);
tab.newDiv();


-----------------------------

tab2.newTitle("Silent Aim");
tab2.newDiv();
tab2.newCheckbox("Enable", false, function(State)
    if State then
        Aiming.Enabled = true
    else
        Aiming.Enabled = false
    end
end)

tab2.newTextbox("Notification Toggle", "Lowercase", function(state)
    _G.nigger = state
end)
tab2.newTextbox("No Notification", "Lowercase", function(state)
    _G.Shitkid = state
end)
tab2.newTitle("Customize");
tab2.newDiv();



tab2.newCheckbox("FOV Circle", false, function(State)
    if State then
        Aiming.ShowFOV = true
    else
        Aiming.ShowFOV = false
    end
	print("Toggled", value)
end)
tab2.newSlider("FOV Size", 60, 0, 300, function(Text)
    Aiming.FOV = Text
end)
tab2.newSlider("FOV Sides", 50, 3, 50, function(Text)
    Aiming.FOVSides = Text
end)


tab2.newTitle("Settings");
tab2.newDiv();

tab2.newCheckbox("Visible Check", false, function(state)
Aiming.VisibleCheck = state
end)
tab2.newCheckbox("Auto Settings", false, function(state)

end)

tab2.newCheckbox("Crew Check", false, function(state)

end)
tab2.newCheckbox("Ground Check", false, function(state)
Aiming.GroundCheck = state
end)
tab2.newDropdown("Body Parts", "Select", {
	"Head",
	"HumanoidRootPart",
	"UpperTorso",
	"LowerTorso",
	}, function(state)
		Aiming.TargetPart = state
	end)

    bruh = false
	plr = game.Players.LocalPlayer
	mouse = plr:GetMouse()
	mouse.KeyDown:connect(function(key)
		if key == _G.Keyyy and bruh == false then
			bruh = true
			ESP.Enabed = true
		elseif key == _G.Keyyy and bruh == true then
			bruh = false
ESP:Toggle()
		end
	end)

    tab5.newTitle("ESP");
    tab5.newDiv();
    tab5.newCheckbox("Enable", ESP.Enabled, function(state)
        ESP.Enabled = state
    end)
    tab5.newTextbox("Keybind", "Lowercase", function(state)
        _G.Keyyy = state
    end)
    tab5.newDiv();
    
    tab5.newTitle("Settings");
    tab5.newCheckbox("Tracer", ESP.Tracers, function(state)
        ESP.Tracers = state;
    end)
    tab5.newCheckbox("Boxes", ESP.Boxes, function(state)
        ESP.Boxes = state;
    end)
    tab5.newCheckbox("Names", ESP.Names, function(state)
        ESP.Names = state;
    end)

    shop.newTitle("Animations");
    shop.newDiv();
    shop.newButton("Astronaut", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=891621366"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=891633237"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=891667138"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=891636393"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=891627522"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=891609353"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=891617961"
    end)
    shop.newButton("Bubbly", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=910004836"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=910009958"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=910034870"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=910025107"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=910016857"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=910001910"
        Animate.swimidle.SwimIdle.AnimationId = "http://www.roblox.com/asset/?id=910030921"
        Animate.swim.Swim.AnimationId = "http://www.roblox.com/asset/?id=910028158"
    end)
    shop.newButton("Cartoony", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=742637544"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=742638445"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=742640026"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=742638842"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=742637942"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=742636889"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=742637151"
    end)
    shop.newButton("Elder", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=845397899"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=845400520"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=845403856"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=845386501"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=845398858"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=845392038"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=845396048"
    end)
    shop.newButton("Knight", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=657595757"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=657568135"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=657552124"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=657564596"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=658409194"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=658360781"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=657600338"
    end)
    shop.newButton("Levitation", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=616006778"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=616008087"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=616013216"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=616010382"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=616008936"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=616003713"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=616005863"
    end)
    shop.newButton("Mage", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=707742142"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=707855907"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=707897309"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=707861613"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=707853694"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=707826056"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=707829716"
    end)
    shop.newButton("Ninja", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=656117400"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=656118341"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=656121766"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=656118852"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=656117878"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=656114359"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=656115606"
    end)
    shop.newButton("Pirate", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=750781874"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=750782770"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=750785693"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=750783738"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=750782230"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=750779899"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=750780242"
    end)

    shop.newButton("Robot", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=616088211"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=616089559"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=616095330"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=616091570"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=616090535"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=616086039"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=616087089"
    end)
    shop.newButton("Stylish", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=616136790"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=616138447"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=616146177"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=616140816"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=616139451"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=616133594"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=616134815"    end)
    shop.newButton("Superhero", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=616111295"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=616113536"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=616122287"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=616117076"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=616115533"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=616104706"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=616108001"
    end)
    shop.newButton("Toy", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=782841498"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=782845736"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=782843345"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=782842708"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=782847020"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=782843869"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=782846423"
    end)
    shop.newButton("Vampire", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=1083445855"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=1083450166"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=1083473930"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=1083462077"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=1083455352"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=1083439238"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=1083443587"
    end)
    shop.newButton("Werewolf", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=1083195517"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=1083214717"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=1083178339"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=1083216690"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=1083218792"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=1083182000"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=1083189019"
    end)
    shop.newButton("Zombie", function()
        local Animate = game.Players.LocalPlayer.Character.Animate
        Animate.idle.Animation1.AnimationId = "http://www.roblox.com/asset/?id=616158929"
        Animate.idle.Animation2.AnimationId = "http://www.roblox.com/asset/?id=616160636"
        Animate.walk.WalkAnim.AnimationId = "http://www.roblox.com/asset/?id=616168032"
        Animate.run.RunAnim.AnimationId = "http://www.roblox.com/asset/?id=616163682"
        Animate.jump.JumpAnim.AnimationId = "http://www.roblox.com/asset/?id=616161997"
        Animate.climb.ClimbAnim.AnimationId = "http://www.roblox.com/asset/?id=616156119"
        Animate.fall.FallAnim.AnimationId = "http://www.roblox.com/asset/?id=616157476"

    end)






    local userInput = game:service('UserInputService')
    local runService = game:service('RunService')
    
    userInput.InputBegan:connect(function(Key)
        if Key.KeyCode == _G.AntiLock then
            Enabled = not Enabled
            if Enabled == true then
                repeat
                    game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame = game.Players.LocalPlayer.Character.HumanoidRootPart.CFrame + game.Players.LocalPlayer.Character.Humanoid.MoveDirection * getgenv().Multiplier
                    runService.Stepped:wait()
                until Enabled == false
            end
        end
    end)

tab4.newTitle("WalkSpeed");
tab4.newDiv();

tab4.newTextbox("Keybind", "Lowercase", function(state)
    _G.Lol = state
end)

tab4.newSlider("Speed", 16, 0, 1000, function(state)
    _G.Speeed = state
end)

tab4.newTitle("Anti Lock");
tab4.newDiv();
tab4.newTextbox("Keybind", "Lowercase", function(state)
    _G.AntiLock = state
end)
tab4.newTextbox("Speed", "0.3", function(state)
    getgenv().Multiplier = state
end)


tab4.newTitle("Others");
tab4.newDiv();
tab4.newSlider("FOV", 70, 0, 120, function(state)
    game.Workspace.Camera.FieldOfView = state

end)


mainGUI.openTab(tab);
