/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as React from 'react'
import { Grid, Column, SwitchButton, BrowserSelect, ContentToggle } from 'brave-ui'
import * as shieldActions from '../../types/actions/shieldsPanelActions'
import { BlockOptions } from '../../types/other/blockTypes'

export interface Props {
  controlsOpen: boolean
  adsTrackers: string
  shieldsEnabled: BlockOptions
  httpsEverywhere: BlockOptions
  javascript: BlockOptions
  blockAdsTrackers: shieldActions.BlockAdsTrackers
  controlsToggled: shieldActions.ControlsToggled
  httpsEverywhereToggled: shieldActions.HttpsEverywhereToggled
  javascriptToggled: shieldActions.JavascriptToggled
}

export default class BraveShieldsControls extends React.Component<Props, object> {
  constructor (props: Props) {
    super(props)
    this.onChangeAdControl = this.onChangeAdControl.bind(this)
    this.onChangeCookieControl = this.onChangeCookieControl.bind(this)
    this.onToggleControls = this.onToggleControls.bind(this)
    this.onToggleHTTPSEverywhere = this.onToggleHTTPSEverywhere.bind(this)
    this.onToggleJavaScript = this.onToggleJavaScript.bind(this)
  }

  onChangeAdControl (e: HTMLSelectElement) {
    this.props.blockAdsTrackers(e.target.value)
  }

  onChangeCookieControl () {
    // TODO: @cezaraugusto
  }

  onToggleControls () {
    this.props.controlsToggled(!this.props.controlsOpen)
  }

  onToggleHTTPSEverywhere () {
    this.props.httpsEverywhereToggled()
  }

  onToggleJavaScript () {
    this.props.javascriptToggled()
  }

  render () {
    const { shieldsEnabled, adsTrackers, controlsOpen, httpsEverywhere, javascript } = this.props
    return (
      <Grid
        id='braveShieldsControls'
        background='#eee'
        padding='10px 10px 0'
        gap='10px 5px'
      >
        <Column>
          <ContentToggle
            withSeparator={true}
            open={controlsOpen}
            summary='Advanced Controls'
            onClick={this.onToggleControls}
          >
            <BrowserSelect
              disabled={shieldsEnabled === 'block'}
              titleName='Ad Control'
              value={adsTrackers}
              onChange={this.onChangeAdControl}
            >
              {/* TODO needs "show brave ads" */}
              <option value='allow'>Block Ads</option>
              <option value='block'>Allow Ads and Tracking</option>
            </BrowserSelect>
            {/* TODO @cezaraugusto */}
            <BrowserSelect
              disabled={shieldsEnabled === 'block'}
              titleName='Cookie Control'
              value='someVALUE'
              onChange={this.onChangeCookieControl}
            >
              <option value='SOME'>Block 3rd Party Cookies</option>
              <option value='SOME'>Allow All Cookies</option>
              <option value='SOME'>Block All Cookies</option>
            </BrowserSelect>
            <Grid
              gap='10px 5px'
              padding='5px 0'
            >
              <Column>
                {/* TODO @cezaraugusto */}
                <SwitchButton
                  id='httpsEverywhere'
                  disabled={shieldsEnabled === 'block'}
                  rightText='HTTPS Everywhere'
                  checked={shieldsEnabled === 'allow' && httpsEverywhere === 'allow'}
                  onChange={this.onToggleHTTPSEverywhere}
                />
              </Column>
              <Column>
                {/* TODO @cezaraugusto */}
                <SwitchButton
                  id='blockScripts'
                  disabled={shieldsEnabled === 'block'}
                  rightText='Block Scripts'
                  checked={shieldsEnabled === 'allow' && javascript === 'block'}
                  onChange={this.onToggleJavaScript}
                />
              </Column>
              <Column>
                {/* TODO @cezaraugusto */}
                <SwitchButton
                  id='fingerprintingProtection'
                  disabled={shieldsEnabled === 'block'}
                  rightText='Fingerprinting Protection'
                />
              </Column>
              <Column>
                {/* TODO @cezaraugusto */}
                <SwitchButton
                  id='blockPhishingMalware'
                  disabled={shieldsEnabled === 'block'}
                  rightText='Block Phishing/Malware'
                />
              </Column>
            </Grid>
          </ContentToggle>
        </Column>
      </Grid>
    )
  }
}
