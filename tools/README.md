# Shakujo tools

## Requirements

* JDK 10

## usage

```bash
cd tools/codegen
./gradlew generate
```

* generated files will place under..
  * `<repository-root>/model/include`
  * `<repository-root>/model/src`
* generated files will have the following comment line below the license header
  * `// DON'T EDIT THIS GENERATED FILE`
  * note that, these overwrites the existing files
