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
        saveCounter();
    }
}, 1000);