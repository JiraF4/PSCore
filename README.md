# MissionMakerForDummies
- Core mode for PS mods, mainly contain modded editor attributes save struct.
- Also manual markers, for workbench and GM.
- And some SQF interpretator.

[Workshop page](https://reforger.armaplatform.com/workshop/5EE9238E61637E47-PSCore)

# Modded save system
Default attributes system do not support string saving at all.

12 bytes in vector = 12 characters with is a little it small.

I don't know why vanilla can't just serialize all attributes to string, json is an string where is the problem?

Also attribute addrese by "id" whhit is just order nunmber of attribute and **any change will break old saves**...

Well, at least it work with XBOX...

I will rework it latter, but there is much of work.

# manual markers
Custom marker for GM create self on map when MapOpen event triggered, update self position every frame.

If you want to know how to add attributes to GM, it's a good point for start.

There is many of attributes include strings and custom layouts.

All atributes require their own class, line in Edit.conf and layout that can work with atrribute data.

# SQF interpretator
Yes, SQF interpretator and it's work but still in WIP.

Parse/"Compile" code at runtime, can be executed unsheduled or scheduled (1ms per frame)

All the code execute on vm (scripted on enfusion vm)

Right now there's:

- Variable: local, global, array, number (float), string, entity, codeblock
- Statements: if, else, while, ForEach
- commands: almost none right now...

Why SQF if there's enscript? 

Becase I love SQF, and it can be stored in mission file.

Is it slow? 

Yes, don't use it for systems, only for mission one time scripts, btw it can't lag your game in scheduled envorniment.

And two hundreds of entities can be processed every frame without any problem, it's not sooo slow, as you can think.
