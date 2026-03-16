import { createSlice, createAsyncThunk } from '@reduxjs/toolkit'
import { authService } from '../services'

export const loginThunk = createAsyncThunk('auth/login', async (creds, { rejectWithValue }) => {
    try { 
        return await authService.login(creds) 
    }
    catch (err) { 
        return rejectWithValue(err.response?.data?.message || 'Login failed') 
    }
})

export const registerThunk = createAsyncThunk('auth/register', async (data, { rejectWithValue }) => {
    try { 
        return await authService.register(data) 
    }
    catch (err) { 
        return rejectWithValue(err.response?.data?.message || 'Registration failed') 
    }
})

const stored = localStorage.getItem('user')

const authSlice = createSlice({
    name: 'auth',
    initialState: {
        user:stored ? JSON.parse(stored) : null,
        token:localStorage.getItem('token') || null,
        loading:false,
        error:null,
    },
    reducers: {
        logout(state) {
            state.user = null
            state.token = null
            localStorage.removeItem('token')
            localStorage.removeItem('user')
        },
        clearError(state) { 
            state.error = null 
        }
    },
    extraReducers: builder => {
        const pending= state => { 
            state.loading = true; state.error = null 
        }
        const fulfilled = (state, action) => {
            state.loading = false
            state.token= action.payload.token
            state.user= { name: action.payload.name, email: action.payload.email }
            localStorage.setItem('token', action.payload.token)
            localStorage.setItem('user', JSON.stringify(state.user))
        }
        const rejected=(state, action) => { 
            state.loading = false; state.error = action.payload 
        }

        builder
        .addCase(loginThunk.pending,pending)
        .addCase(loginThunk.fulfilled,fulfilled)
        .addCase(loginThunk.rejected,rejected)
        .addCase(registerThunk.pending,pending)
        .addCase(registerThunk.fulfilled,fulfilled)
        .addCase(registerThunk.rejected,rejected)
    }
})

export const { logout, clearError } = authSlice.actions
export default authSlice.reducer