import api from './api'

export const authService = {
    register: (data) => api.post('/auth/register', data).then((r) => r.data),
    login: (data) => {
    console.log('Login request payload:', data)

    return api.post('/auth/login', data)
        .then((r) => {
            console.log('Login response:', r.data)
            console.log('Token received:', r.data.token)
            return r.data
        })
        .catch((err) => {
            console.log('Login error status:', err.response?.status)
            console.log('Login error data:', err.response?.data)
            throw err
        })
}
}

export const userService = {
    getAll: () => api.get('/users').then((r) => r.data),
    getMe: () => api.get('/users/me').then((r) => r.data),
    getById: (id) => api.get(`/users/${id}`).then((r) => r.data),
}

export const groupService = {
    create: (data) => api.post('/groups/create', data).then((r) => r.data),
    getAll: () => api.get('/groups').then((r) => r.data),
    getById: (id) => api.get(`/groups/${id}`).then((r) => r.data),
    addMember: (groupId, userId) =>
        api.post(`/groups/${groupId}/members/${userId}`).then((r) => r.data),
    leave: (groupId) => api.delete(`/groups/${groupId}/leave`).then(r => r.data),
    delete: (groupId) => api.delete(`/groups/${groupId}`).then(r => r.data),
}

export const expenseService = {
    add: (data) => api.post('/expenses/add', data).then((r) => r.data),
    getAll: () => api.get('/expenses/all').then(r => r.data),
    getByGroup: (groupId) => api.get(`/expenses/group/${groupId}`).then((r) => r.data),
    delete: (id) => api.delete(`/expenses/${id}`).then((r) => r.data),
}

export const settlementService = {
    getPlan: (groupId) => api.get(`/settlements/group/${groupId}`).then((r) => r.data),

    record: (groupId, payerId, payeeId, amount) =>
        api
        .post(
            `/settlements/record?groupId=${groupId}&payerId=${payerId}&payeeId=${payeeId}&amount=${amount}`
        )
        .then((r) => r.data),

    getHistory: (groupId) =>
        api.get(`/settlements/group/${groupId}/history`).then((r) => r.data),
}