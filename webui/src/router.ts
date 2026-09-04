import { createRouter, createWebHashHistory } from 'vue-router'
import OverviewView from './views/OverviewView.vue'
import MountView from './views/MountView.vue'
import RotatorView from './views/RotatorView.vue'
import FocuserView from './views/FocuserView.vue'
import AuxiliaryView from './views/AuxiliaryView.vue'
import EncodersView from './views/EncodersView.vue'

export const router = createRouter({
  history: createWebHashHistory(),
  routes: [
    { path: '/', name: 'overview', component: OverviewView },
    { path: '/mount', name: 'mount', component: MountView },
    { path: '/rotator', name: 'rotator', component: RotatorView },
    { path: '/focuser', name: 'focuser', component: FocuserView },
    { path: '/auxiliary', name: 'auxiliary', component: AuxiliaryView },
    { path: '/encoders', name: 'encoders', component: EncodersView },
  ],
})
