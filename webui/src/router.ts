import { createRouter, createWebHashHistory } from 'vue-router'
import SettingsView from './views/SettingsView.vue'
import OverviewView from './views/OverviewView.vue'
import MountView from './views/MountView.vue'
import RotatorView from './views/RotatorView.vue'
import FocuserView from './views/FocuserView.vue'
import AuxiliaryView from './views/AuxiliaryView.vue'
import EncodersView from './views/EncodersView.vue'
import NetworkView from './views/NetworkView.vue'

export const router = createRouter({
  history: createWebHashHistory(),
  routes: [
    { path: '/settings', name: 'settings', component: SettingsView },
    { path: '/', redirect: '/mount' },
    { path: '/overview', name: 'overview', component: OverviewView },
    { path: '/mount', name: 'mount', component: MountView },
    { path: '/rotator', name: 'rotator', component: RotatorView },
    { path: '/focuser', name: 'focuser', component: FocuserView },
    { path: '/auxiliary', name: 'auxiliary', component: AuxiliaryView },
    { path: '/encoders', name: 'encoders', component: EncodersView },
    { path: '/network', name: 'network', component: NetworkView },
  ],
})
