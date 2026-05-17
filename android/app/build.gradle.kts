import java.io.DataInputStream
import java.util.Properties

plugins {
    alias(libs.plugins.androidApplication)
}

val properties = Properties()
val localPropertiesFile = project.rootProject.file("local.properties")
if (localPropertiesFile.exists()) {
    val inputStream = DataInputStream(localPropertiesFile.inputStream())
    properties.load(inputStream)
    inputStream.close()
}
val screenOrientation = properties.getProperty("screenOrientation") ?: "sensorPortrait"

android {
    namespace = "seker.logger.android"

    compileSdk = libs.versions.android.compileSdk.get().toInt()

    defaultConfig {
        minSdk = libs.versions.android.minSdk.get().toInt()

        versionCode = 1
        versionName = "1.0.0"

        applicationId = "seker.logger.android"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
    }

    buildTypes {
        debug {
            manifestPlaceholders["screenOrientation"] = screenOrientation
        }
        release {
            manifestPlaceholders["screenOrientation"] = screenOrientation
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
    lint {
    }
    testOptions {
    }
}

dependencies {
    implementation(project(":logger"))

    implementation(libs.google.material)

    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.androidx.constraintlayout)
    implementation(libs.androidx.lifecycle.livedata.ktx)
    implementation(libs.androidx.lifecycle.viewmodel)

    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.testExt.junit)
    androidTestImplementation(libs.androidx.espresso.core)
}