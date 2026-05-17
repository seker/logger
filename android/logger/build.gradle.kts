plugins {
    alias(libs.plugins.androidLibrary)
    `maven-publish`
}

group = "io.github.seker"
version = "1.0.3-SNAPSHOT"

android {
    namespace = "seker.logger"

    compileSdk = libs.versions.android.compileSdk.get().toInt()

    defaultConfig {
        minSdk = libs.versions.android.minSdk.get().toInt()
        ndkVersion = libs.versions.ndkVersion.get()

        externalNativeBuild {
            cmake {
                cppFlags("-O2 -frtti -fexceptions -std=c++11")
                cFlags("-O2")
            }
        }

        ndk {
            //noinspection ChromeOsAbiSupport
            abiFilters += listOf("armeabi-v7a", "arm64-v8a")
//             abiFilters += listOf("arm64-v8a")
        }

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        consumerProguardFiles("consumer-rules.pro")
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = libs.versions.cmakeVersion.get()
        }
    }
    lint {
        targetSdk = libs.versions.android.targetSdk.get().toInt()
    }
    testOptions {
    }

    publishing {
        singleVariant("release") {
            // 让官方托管源码包生成，完美包含 Java + Kotlin + 自动生成的代码
            withSourcesJar()
        }
    }
}

dependencies {
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.testExt.junit)
    androidTestImplementation(libs.androidx.espresso.core)
}

publishing {
    // publish 到 output
    publications.create<MavenPublication>("Logger") {
        afterEvaluate {
            from(components["release"])
        }

        // 1. 三要素直接配置在 Publication 级别（等号赋值）
        groupId = project.group.toString()
        artifactId = "logger-android"
        version = project.version.toString()

        // 2. 规范的 POM 元数据配置（内部全部改用 .set()）
        pom {
            name.set("logger-android")
            description.set("logger for android")
            url.set("https://github.com/seker/logger")

            licenses {
                license {
                    name.set("GNU LESSER GENERAL PUBLIC LICENSE Version 2.1")
                    url.set("https://github.com/seker/logger/blob/main/LICENSE")
                }
            }

            developers {
                developer {
                    id.set("xinjian")
                    name.set("xinjian.liu")
                    email.set("04070628@163.com")
                }
            }

            scm {
                connection.set("scm:git:git@github.com:seker/logger.git")
                developerConnection.set("scm:git:ssh://github.com:seker/logger.git")
                url.set("https://github.com/seker/logger")
            }
        }
    }
}
