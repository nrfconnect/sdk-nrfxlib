//
// Copyright (c) 2020 Nordic Semiconductor ASA. All Rights Reserved.
//
// The information contained herein is confidential property of Nordic Semiconductor ASA.
// The use, copying, transfer or disclosure of such information is prohibited except by
// express written agreement with Nordic Semiconductor ASA.
//

@Library("CI_LIB") _

HashMap CI_STATE = lib_State.getConfig(JOB_NAME)

properties([
  pipelineTriggers([
    parameterizedCron( [
        ((JOB_NAME =~ /latest\/night\/.*\/master/).find() ? CI_STATE.CFG.CRON.NIGHTLY : ''),
        ((JOB_NAME =~ /latest\/week\/.*\/master/).find() ? CI_STATE.CFG.CRON.WEEKLY : '')
    ].join('    \n') )
  ]),
  ( JOB_NAME.contains('sub/') ? disableResume() :  disableConcurrentBuilds() )
])

pipeline
{
  parameters {
    booleanParam(name: 'RUN_TESTS', description: 'if false skip testing', defaultValue: true)
    booleanParam(name: 'RUN_BUILD', description: 'if false skip building', defaultValue: true)
    booleanParam(name: 'RUN_DOWNSTREAM', description: 'if false skip downstream jobs', defaultValue: false)
    string(      name: 'jsonstr_CI_STATE', description: 'Default State if no upstream job', defaultValue: CI_STATE.CFG.INPUT_STATE_STR )
    choice(      name: 'CRON', description: 'Cron Test Phase', choices: CI_STATE.CFG.CRON_CHOICES)
  }

  agent {
    docker {
      image CI_STATE.CFG.IMAGE_TAG
      label CI_STATE.CFG.AGENT_LABELS
    }
  }

  options {
    checkoutToSubdirectory('nrfxlib')
    parallelsAlwaysFailFast()
    timeout(time: CI_STATE.CFG.TIMEOUT.time, unit: CI_STATE.CFG.TIMEOUT.unit)
  }

  environment {
      // ENVs for check-compliance
      GH_TOKEN = credentials('nordicbuilder-compliance-token') // This token is used to by check_compliance to comment on PRs and use checks
      GH_USERNAME = "NordicBuilder"
      COMPLIANCE_ARGS = "-r nrfconnect/sdk-nrfxlib"

      // Build all custom samples that match the ci_build tag
      SANITYCHECK_OPTIONS = "--board-root $WORKSPACE/nrf/boards --testcase-root $WORKSPACE/nrf/samples --testcase-root $WORKSPACE/nrf/applications --build-only --disable-unrecognized-section-test -t ci_build --inline-logs"
      ARCH = "-a arm"
      LC_ALL = "C.UTF-8"

      // ENVs for building (triggered by sanitycheck)
      ZEPHYR_TOOLCHAIN_VARIANT = 'gnuarmemb'
  }

  stages {
    stage('Load')     { steps { script { CI_STATE = lib_State.load('NRFXLIB', CI_STATE) }}}
    stage('Checkout') { steps { script {
        CI_STATE.SELF.REPORT_SHA = lib_Main.checkoutRepo(CI_STATE.SELF.GIT_URL, "nrfxlib", CI_STATE.SELF, false)
        lib_West.AddManifestUpdate("NRFXLIB", 'nrfxlib', CI_STATE.SELF.GIT_URL, CI_STATE.SELF.GIT_REF, CI_STATE)
    } } }
    stage('West Update') {
      when { expression { CI_STATE.SELF.RUN_TESTS || CI_STATE.SELF.RUN_BUILD } }
      steps { script {
        lib_Status.set('PENDING', 'NRFXLIB', CI_STATE)
        lib_Main.checkoutRepo(CI_STATE.NRF.GIT_URL, "nrf", CI_STATE.NRF, true)
        lib_West.InitUpdate('nrf', 'ci-tools')
        lib_West.ApplyManifestUpdates(CI_STATE)
      }}
    }
    stage('Run compliance check') {
      when { expression { CI_STATE.SELF.RUN_TESTS } }
      steps {
        dir('nrfxlib') {
          script {
            // If we're a pull request, compare the target branch against the current HEAD (the PR), and also report issues to the PR
            def BUILD_TYPE = lib_Main.getBuildType(CI_STATE.SELF)
            if (BUILD_TYPE == "PR") {
              COMMIT_RANGE = "$CI_STATE.SELF.MERGE_BASE..$CI_STATE.SELF.REPORT_SHA"
              COMPLIANCE_ARGS = "$COMPLIANCE_ARGS -p $CHANGE_ID -S $CI_STATE.SELF.REPORT_SHA -g"
              println "Building a PR [$CHANGE_ID]: $COMMIT_RANGE"
            }
            else if (BUILD_TYPE == "TAG") {
              COMMIT_RANGE = "tags/${env.BRANCH_NAME}..tags/${env.BRANCH_NAME}"
              println "Building a Tag: " + COMMIT_RANGE
            }
            // If not a PR, it's a non-PR-branch or master build. Compare against the origin.
            else if (BUILD_TYPE == "BRANCH") {
              COMMIT_RANGE = "origin/${env.BRANCH_NAME}..HEAD"
              println "Building a Branch: " + COMMIT_RANGE
            }
            else {
                assert condition : "Build fails because it is not a PR/Tag/Branch"
            }

            // Run the compliance check
            try {
              sh "../tools/ci-tools/scripts/check_compliance.py $COMPLIANCE_ARGS --commits $COMMIT_RANGE"
            }
            finally {
              junit 'compliance.xml'
              archiveArtifacts artifacts: 'compliance.xml'
            }
          }
        }
      }
    }
    stage('Build samples') {
      when { expression { CI_STATE.SELF.RUN_BUILD } }
      steps {
          echo "No Samples to build yet."
      }
    }
    stage('Trigger Downstream Jobs') {
      when { expression { CI_STATE.SELF.RUN_DOWNSTREAM } }
      steps { script { lib_Stage.runDownstream(JOB_NAME, CI_STATE) } }
    }
  }
  post {
    // This is the order that the methods are run. {always->success/abort/failure/unstable->cleanup}
    always {
      echo "always"
      script { if ( !CI_STATE.SELF.RUN_BUILD || !CI_STATE.SELF.RUN_TESTS ) { currentBuild.result = "UNSTABLE"}}
    }
    success {
      echo "success"
      script { lib_Status.set("SUCCESS", 'NRFXLIB', CI_STATE) }
    }
    aborted {
      echo "aborted"
      script { lib_Status.set("ABORTED", 'NRFXLIB', CI_STATE) }
    }
    unstable {
      echo "unstable"
    }
    failure {
      echo "failure"
      script { lib_Status.set("FAILURE", 'NRFXLIB', CI_STATE) }
    }
    cleanup {
        echo "cleanup"
        cleanWs()
    }
  }
}
