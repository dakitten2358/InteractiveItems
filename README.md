# Interactive Items

*Fair Warning:  This is a C++ plugin for UE4*

Quick demo video:  https://youtu.be/hkKlg-VNaKM

## How To

### Setup Interactive Items

Under `Class Settings` / `Implemented Interfaces` add the interface `Interaction Interface`.  Under `Interfaces`, provide implementations for:
* `IsInteractionTarget` Can this item be currently interacted with?
* `GetInteractionType` Is this an instant interaction, or one that requires time?
* `GetInteractionDetails` Text + image for the interaction
* `GetInteractionWorldLocation` By default, objects will use their Actor location, but can be overridden using this

To respond to the interaction, add the event `Interact` from the `Interaction Interface` to your blueprint graph.  This will be called with `Authority`, so pay attention if you need to modify something on the clients (e.g. hiding the mesh, etc) that isn't auto replicated.

Highlighting of the item can be controlled via the events `StartInteractionHighlight` and `StopInteractionHighlight`.  These will only be called on the local client.

### Setup UI

Create a Widget that will represent your interaction widget (`InteractionWidget` is the default, and can be used).  Ensure that it implements the `Interaction Widget Interface`.

Create a Widget that will represent your main HUD UI, and add your interaction widget to it.  Ensure that the UI implements the `Interaction Widget Interface` as well.

Your HUD object should create the UI widget, and add it to the viewport.  Add an `InteractionSensingComponent` to your HUD object.  `OnInteractionTargetUpdated` will get called when there is a change in the interaction target.  You can check the source via `Stimulus Interaction Type`, which will be one of `Show` (a new object is being shown), `Update` (need to reposition the UI, update progress, etc), and `Hide`.  These should be forwarded to the UI via the `Interaction Widget Interface` calls `ShowInteractionWidget`, `UpdateInteractionWidget` and `HideInteractionWidget`.

### Interaction

Assign an action mapping for interaction.  Under the settings for the plugin make sure the action name matches.  Add an `InteractionInvokerComponent` to your pawn/character, and connect the `InputAction`'s `Pressed` and `Released` to `BeginInteraction` and `CancelInteraction`.

## Additional

### Tracing methods

There's three tracing methods you can use:
* `CameraAndPawn` This is the default, and will trace from the camera to the center of the screen to find a target, and then will trace again from the actor's location to the target found by the initial trace.  This helps w/ windows, etc. where the player can see the interactive item, but may not be able to reach it.  This is ideal for third person over the shoulder/offset cameras.
* `Camera` This simply checks from the camera to the center of the screen.  This is fine for first person modes.
* `Pawn` Checks from the actor location in the direction the actor is facing.  Best for top down (or nearly) views.

### Debugging

`ShowTraces` will turn on a per frame trace.

### This seems like a lot of work!

It is!  I'll be adding some helper classes to provide basic implementations of really common things, but I want to avoid bloating the plugin with classes and content that _should_ be replaced by the game itself.

### How did you do the progress circle?

https://www.tomlooman.com/circular-progress-bar-for-umg/

### Highlighting

I didn't include the material used for highlighting as it's not mine; I'm currently using Local Outlines (https://www.unrealengine.com/marketplace/en-US/slug/local-outlines).  I intend to replace this with my own method and include it in the plugin.
