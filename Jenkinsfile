pipeline {
    agent {
        docker {
            image 'nautilus/oltp-sandbox'
            label 'docker'
            args '--cap-add SYS_PTRACE'
        }
    }
    environment {
        GITHUB_URL = 'https://github.com/project-tsurugi/shakujo'
        GITHUB_CHECKS = 'tsurugi-check'
        BUILD_PARALLEL_NUM="""${sh(
                returnStdout: true,
                script: 'grep processor /proc/cpuinfo | wc -l'
            )}"""
    }
    stages {
        stage ('Prepare env') {
            steps {
                sh '''
                    ssh-keyscan -t rsa github.com > ~/.ssh/known_hosts
                '''
            }
        }
        stage ('checkout master') {
            steps {
                checkout scm
                sh '''
                    git clean -dfx
                    git submodule sync --recursive
                    git submodule update --init --recursive
                '''
            }
        }
        stage ('Build') {
            steps {
                sh '''
                    mkdir build
                    cd build
                    cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
                    make all -j${BUILD_PARALLEL_NUM}
                '''
            }
        }
        stage ('Test') {
            environment {
                GTEST_OUTPUT="xml"
                ASAN_OPTIONS="detect_stack_use_after_return=true"
            }
            steps {
                sh '''
                    cd build
                    make test ARGS="--verbose"
                '''
            }
        }
        stage ('Doc') {
            steps {
                sh '''
                    cd build
                    make doxygen > doxygen.log 2>&1
                    zip -q -r shakujo-doxygen doxygen/html
                '''
            }
        }
        stage ('Coverage') {
            environment {
                GCOVR_COMMON_OPTION='-e ../third_party/ -e ../.*/test.* -e ../.*/examples.* -e .*/antlr.*'
            }
            steps {
                sh '''
                    cd build
                    mkdir gcovr-xml gcovr-html
                    gcovr -j ${BUILD_PARALLEL_NUM} -r .. --xml ${GCOVR_COMMON_OPTION} -o gcovr-xml/shakujo-gcovr.xml
                    gcovr -j ${BUILD_PARALLEL_NUM} -r .. --html --html-details --html-title "shakujo coverage" ${GCOVR_COMMON_OPTION} -o gcovr-html/shakujo-gcovr.html
                    zip -q -r shakujo-coverage-report gcovr-html
                '''
            }
        }
        stage ('Lint') {
            steps {
                sh '''#!/bin/bash
                    python tools/bin/run-clang-tidy.py -quiet -export-fixes=build/clang-tidy-fix.yaml -p build -extra-arg=-Wno-unknown-warning-option -header-filter=$(pwd)'/(include|common|model|parser|analyzer|examples)/.*\\.h$' $(pwd)'/(src|(common|model|parser|analyzer)/src|examples)/.*' > build/clang-tidy.log 2> build/clang-tidy-error.log
                '''
            }
        }
        stage ('Graph') {
            steps {
                sh '''
                    cd build
                    cp ../cmake/CMakeGraphVizOptions.cmake .
                    cmake --graphviz=dependency-graph/shakujo.dot ..
                    cd dependency-graph
                    dot -T png shakujo.dot -o shakujo.png
                '''
            }
        }
    }
    post {
        always {
            junit keepLongStdio: true, allowEmptyResults: true, testResults: '**/*_gtest_result.xml'
            warnings consoleParsers: [
                [parserName: 'GNU Make + GNU C Compiler (gcc)'],
            ]
            warnings parserConfigurations: [
                [parserName: 'Clang (LLVM based)', pattern: 'build/clang-tidy.log'],
            ], unstableTotalAll: '0'
            warnings parserConfigurations: [
                [parserName: 'Doxygen', pattern: 'build/doxygen.log'],
            ]
            cobertura autoUpdateHealth: false, autoUpdateStability: false, coberturaReportFile: 'build/gcovr-xml/shakujo-gcovr.xml', failNoReports: false, failUnhealthy: false, failUnstable: false, maxNumberOfBuilds: 0, zoomCoverageChart: false
            archiveArtifacts allowEmptyArchive: true, artifacts: 'build/shakujo-coverage-report.zip, build/shakujo-doxygen.zip, build/clang-tidy.log, build/clang-tidy-fix.yaml, build/dependency-graph/shakujo.png', onlyIfSuccessful: true
            notifySlack('tsurugi-dev')
        }
    }
}
