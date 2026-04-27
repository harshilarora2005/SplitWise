import { useDispatch, useSelector } from 'react-redux'
import { AuthContext } from './AuthContext'
import { loginThunk, registerThunk, logout } from '../store/authSlice'

export function AuthProvider({ children }) {
    const dispatch = useDispatch()
    const { user, token, loading, error } = useSelector((state) => state.auth)

    return (
        <AuthContext.Provider
        value={{
            user,
            token,
            loading,
            error,
            login: (creds) => dispatch(loginThunk(creds)).unwrap(),
            register: (data) => dispatch(registerThunk(data)),
            logout: () => dispatch(logout()),
        }}
        >
        {children}
        </AuthContext.Provider>
    )
}