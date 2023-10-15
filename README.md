# Astyanax
This project is my first Solo game that I made in C++ based on the Game [Astyanax](https://en.wikipedia.org/wiki/The_Astyanax). See about my [version below](##-Core_Mechanics)

## Version 1.1
You can play it [here](https://github.com/DijiOfficial/Astyanax/releases/tag/v1.1) by downloading the .rar file.

Press "i" or "esc" for the controls and enjoy!

![menu reference](Resources/PickUpsFinal.png)
![gameplay reference](Resources/gameplay.png)
![gameplay reference](Resources/gameplay2.png)

## Astynax The Gane

This game is a close replica of the Original Game Astyanax made for the Arcade and later NES, with a small modifications.

### Core Mechanics

You will be playing as Astyanax the hero of this [Story](https://en.wikipedia.org/wiki/The_Astyanax), who is kidnapped by a fairy and transported to another dimension where he has to face the evil wizard Blackhorn and his armies who kidnapped the princess, the only person who can send him back to his world. 

In the original version you can [drop](####-Drops) (more on those later) a weapon mod which will automatically change your weapon to the next in the list of three. The Axe, Spear and Sword. And the only way to change weapon is to meet Cutie the fairy who kidnapped you or drop another wepon mod. See weapon effects bellow:

#### Weapons
![original weapon effects](Resources/OriginalEffectivness.png)

I changed this mechanic as to provide a more diverse gameplay experience. At the beginning you have the option of choosing between the three weapons and their effects have been altered slightly as having anything other than the Sword in the original is weaker.

![original weapon effects](Resources/Weapons-Effectiveness.PNG)

Now, what does this table mean? You have two ways of dealing damage, physical and magical. Physical damage is simple, hit the [ennemies](#-Ennemies) with your [Weapon](####-Weapons), the damage dealt is calculated based on your current Strength * Physical Damage multiplier of your [Weapon](####-Weapons). Your current Strength is indicated by the yellow bar in the HUD. When you attack you have cooldown period during which your strength regenerates, attacking again when your strength is not at max will result in lower damage output.

To deal magical damage you first need to choose your [Spell](####-Spells) using "w" to switch Spell, the default one is the [Fireball](#####-Fireball). Press "z" to cast a Spell, once again the damage dealt is calculated as follow: current Spell base damage * Spell damage multiplier of your [Weapon](####-Weapons). When you Cast a Spell you consume "Mana" starting with 20 points the Mana consumption is calculated as follow: Spell base mana cost * Spell cost multiplier of your [Weapon](####-Weapons).

#### Spells

Now that you are versed in how to effectively murder the [ennemies](#-Ennemies), which Spells are available? You currently hace three spells available the [Fireball](#####-Fireball), the [Time Freeze](#####-Time-Freeze) and the [Lightning Bolt](#####-Lightning-Bolt). You can switch between them using "z" and cast them using "c".

![Powers](Resources/Powers.png)

The selected Spell will be shown by an icon in the bottom left corner of the HUD.

##### Fireball

The Fireball is the default Spell when you start:

![fireball](Resources/fireball.png)

It's characteristics are as follow:
 - Base Damage of 25
 - Base Mana cost of 3
 - Shoots 8 FireBalls in a circle around the player
 - Each FireBall can deal damage but will be consummed after a hit
 - Effective in overwhelming situation

##### Time Freeze

The Time Freeze freezes everything for a couple of seconds.

It's characteristics are as follow:
 - Base Damage of 0
 - Base Mana cost of 1
 - Freezes everything except the player
 - Frozen [ennemies](#-Ennemies) cannot deal damage, but can still be damaged
 - Frozen projectiles won't deal damage and can be destroyed with physical attacks
 - Effective with the Spear [Weapon](####-Weapons) to run past the [ennemies](#-Ennemies)

##### Lightning Bolt

The Lightning Bolt last but not least is the most powerfull Spell but also the more costly choice.

It's characteristics are as follow:
 - Base Damage of 45
 - Base Mana cost of 5
 - Obliterates everything except the player
 - Always Effective

#### Drops

You have already heard about the weapon mod drop from the [Core Mechanics](###-Core-Mechanics) section, but what is it? It's origninally one of Five [Drops](####-Drops), which I removed in my version for a more diverse gameplay option.

The remaining 4 drops are the [Strength](#####-Strength) upgrade, the [Swing Speed](#####-Swing-Speed) upgrade, the [Health](#####-Health-Potion) potion and the [Mana](#####-Mana-Potion) Potion.
![Drops](Resources/PickUpsFinal.png)

Drops are Pre-Generated in the [First Level](###-First-Level) and are represented by standing statues: 
![Statue](Resources/statue.png)
