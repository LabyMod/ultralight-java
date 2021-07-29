/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 - 2021 LabyMedia and contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

console.log("Example script!");

// Thanks to making the JavaAPI available in ExampleLoadListener, Java access is fully possible!
const File = java.importClass("java.io.File");

const FileInputStream = java.importClass("java.io.FileInputStream");
const InputStreamReader = java.importClass("java.io.InputStreamReader");
const BufferedReader = java.importClass("java.io.BufferedReader");

const FileWriter = java.importClass("java.io.FileWriter");

// You can interact with Java objects and classes as if they would be native Javascript objects
const counterFile = new File("counter.txt");

let counter = 0;
if(!counterFile.exists()) {
    console.log(`${counterFile} does not exist yet, starting from 0!`);
} else {
    // Read the value from the counter
    const reader = new BufferedReader(new InputStreamReader(new FileInputStream(counterFile)));
    const line = reader.readLine();
    reader.close();

    if(line === null) {
        console.error("Empty counter.txt, starting from 0!");
    } else {
        try {
            counter = parseInt(line, 10);
            if(isNaN(counter)) {
                console.warn("Counter was NaN, starting from 0!");
                counter = 0;
            } else {
                console.log(`Counter loaded successfully, starting from ${counter}`);
            }
        } catch (e) {
            console.error(`Corrupted counter.txt, could not convert ${line} to an int, starting from 0!`);
        }
    }
}

/**
 * Saves the counter to the counter.txt file.
 */
function saveCounter() {
    console.log(`Saving counter value ${counter}`);

    // Save the counter
    const writer = new FileWriter(counterFile);
    writer.write(counter.toString(10));
    writer.close();
}

setInterval(() => {
    console.log(`Counter now at ${++counter}`);
    if(counter % 10 === 0) {
        // Save the counter every 10 seconds.
        // (Good enough for demo purposes)
        saveCounter();
    }
}, 1000);

// Import the interaction class
const JSInteraction = java.importClass("com.labymedia.ultralight.lwjgl3.opengl.js.JSInteraction");

// Instantiate using the varargs constructor
const interaction = new JSInteraction("Hello, World!", "How are you doing?", "I hope you are fine.");

// Call the demo methods
const messageList = interaction.getMessageList();
const messageArray = interaction.getMessageArray();

// Since the list is not a Javascript own type, we have to use an indexed for loop
for(let i = 0; i < messageList.size(); i++) {
    console.log(`messageList[${i}] = ${messageList.get(i)}`);
}

// The array however has been fully translated
for(const message of messageArray) {
    console.log(`messageArray[@] = ${message}`)
}

// Methods will automatically be translated to functional interfaces if the interface is annotated
// with @FunctionalInterface
interaction.useConsumer((messages) => {
    console.log(`Messages in consumer lambda: messages = [${messages.join(", ")}]`)
});
